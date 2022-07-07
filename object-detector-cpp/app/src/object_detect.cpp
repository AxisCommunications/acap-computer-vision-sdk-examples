/**
 * Copyright (C) 2021 Axis Communications AB, Lund, Sweden
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include <chrono>
#include <fstream>
#include <grpcpp/grpcpp.h>
#include <iomanip>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

#include "serving_client.hpp"

using namespace std;
using namespace cv;
using namespace chrono;
using namespace grpc;

const int IMG_WIDTH = 480;
const int IMG_HEIGHT = 320;
constexpr size_t IMG_NBR_BYTES = IMG_WIDTH * IMG_HEIGHT;
const float CONFIDENCE_CUTOFF = 0.3;

vector<string> get_classes_from_file() {
  string str;
  vector<string> classes;
  ifstream file(getenv("OBJECT_LIST_PATH"));

  if (!file) {
    cerr << "Cannot open classes file" << endl;
    exit(EXIT_FAILURE);
  }

  int expect = 0;
  char unknown[40];
  while (getline(file, str)) {
    int pos = str.find(" ");
    if (pos > 0) {
      int id = atoi(str.substr(0, pos).c_str());
      while (expect < id) {
        snprintf(unknown, sizeof(unknown), "%d  unknown", expect++);
        classes.push_back(unknown);
      }
      classes.push_back(str);
      expect++;
    }
  }

  return classes;
}

VideoCapture setup_capture(int nbr_buffers) {
  // Start a stream from 'CAP_PROP_CHANNEL: 1'
  VideoCapture cap(1);
  cap.set(CAP_PROP_FPS, 5);
  cap.set(CAP_PROP_FRAME_WIDTH, IMG_WIDTH);
  cap.set(CAP_PROP_FRAME_HEIGHT, IMG_HEIGHT);
  cap.set(CAP_PROP_FOURCC, VideoWriter::fourcc('R', 'G', 'B', '3'));

  // Request one extra in-flight buffer to reach target FPS
  cap.set(CAP_PROP_UNIMATRIX_MAX_BUFFERS, nbr_buffers + 1);

  // Validate all properties by trying to grab one frame
  try {
    cap.grab();
  } catch (const std::exception &e) {
    cerr << "Failed to open stream: " << e.what() << endl;
    exit(EXIT_FAILURE);
  }

  if (nbr_buffers > cap.get(CAP_PROP_UNIMATRIX_MAX_BUFFERS)) {
    cerr << "Insufficient UniMatrix Buffers!" << endl;
    exit(EXIT_FAILURE);
  }

  return cap;
}

void postprocess(PredictResponse &response, const vector<string> &classes) {
  OutMap &outputs = *response.mutable_outputs();

  const float *boxes =
      (const float *)outputs["TFLite_Detection_PostProcess"].tensor_content().data();
  const float *objects =
      (const float *)outputs["TFLite_Detection_PostProcess:1"].tensor_content().data();
  const float *confidences =
      (const float *)outputs["TFLite_Detection_PostProcess:2"].tensor_content().data();
  const int nelm = 50;

  cout << fixed << setprecision(2);

  for (int i = 0; i < nelm; i++) {
    const string object = classes[round(objects[i])];
    const float &confidence = confidences[i];
    const float *pbox = boxes + 4 * i;

    // Stop when confidence is too low
    if (confidence <= CONFIDENCE_CUTOFF || confidence > 1.0) {
      break;
    }

    cout << "Object: " << object;
    cout << ", Confidence: " << confidence;
    cout << ", Box: [" << pbox[0] << ", " << pbox[1] << ", " << pbox[2] << ", "
         << pbox[3] << "]" << endl;
  }
}

inline int64_t ms_time(steady_clock::time_point start,
                       steady_clock::time_point end) {
  return duration_cast<milliseconds>(end - start).count();
}

void output_timing_info(steady_clock::time_point start,
                        steady_clock::time_point framecapture_end,
                        steady_clock::time_point grpc_end,
                        steady_clock::time_point postprocess_end) {

  cout << "Capture: " << ms_time(start, framecapture_end) << " ms"
       << endl;
  cout << "Inference grpc call: " << ms_time(framecapture_end, grpc_end) << " ms"
       << endl;
  cout << "Postprocess: " << ms_time(grpc_end, postprocess_end) << " ms"
       << endl;
}

string read_text(const char* path)
{
  FILE* fptr = fopen(path, "r");
  if (fptr == nullptr) {
    throw std::runtime_error(strerror(errno) + string(": ") + path);
  }

  fseek(fptr, 0, SEEK_END);
  size_t len = ftell(fptr);
  fseek(fptr, 0, SEEK_SET);
  string content(len + 1, '\0');
  size_t size = fread(&content[0], 1, len, fptr);
  fclose(fptr);
  return content;
}

int main(int argc, char *argv[]) {
  try {
      // Number of outstanding zero-copy buffers (This is a very precious resource)
      constexpr size_t BUFFERS = 2;
      Mat frame[BUFFERS];

      cout << "Start:";
      for (int i=0; i < argc; i++) {
        cout << " " << argv[i];
      }
      cout << endl;

      vector<string> classes = get_classes_from_file();
      VideoCapture cap = setup_capture(BUFFERS);
      string connect_string = string(getenv("INFERENCE_HOST"));

      shared_ptr<Channel> channel;
      if (argc < 2) {
        shared_ptr<ChannelCredentials> creds = InsecureChannelCredentials();
        channel = grpc::CreateChannel(connect_string, creds);
      }
      else
      {
        string root_cert = read_text(argv[1]);
        SslCredentialsOptions ssl_opts = {root_cert.c_str(), "", ""};
        shared_ptr<ChannelCredentials> creds = grpc::SslCredentials(ssl_opts);
        grpc::ChannelArguments args;
        args.SetSslTargetNameOverride("localhost");
        channel = grpc::CreateCustomChannel(connect_string, creds, args);
      }
      ServingClient client(channel);

      string model(getenv("MODEL_PATH"));

      int frame_idx = 0;
      for (;;) {
        Mat &mat = frame[frame_idx++ % BUFFERS];

        auto time_start = steady_clock::now();

        // Blocking read
        cap.read(mat);

        auto time_framecapture_end = steady_clock::now();

        // Make sure capture succeeded
        if (mat.empty())
          throw std::runtime_error("Failed to fetch frame");

        uint32_t seqnum = cap.get(CAP_PROP_POS_FRAMES);
        // Write info about frame
        cout << "Caught frame " << setw(2) << seqnum << " " << mat.cols << "x"
            << mat.rows << endl;

        cout << "Connecting to: " << connect_string << endl;

        string output;
        // Call the inference server
        auto maybe_response = client.callPredict(model, mat, output);

        auto time_grpc_end = steady_clock::now();

        cout << output << endl;

        // Run postprocessing if we got a valid result
        if (maybe_response) {
          auto response = maybe_response.value();
          postprocess(response, classes);
        }

        auto time_postprocess_end = steady_clock::now();

        output_timing_info(time_start, time_framecapture_end,
          time_grpc_end, time_postprocess_end);

        cout << endl;
      }

      return 0;
  } catch (const exception &e) {
    cerr << "Exception: " << e.what() << endl;
  }
}