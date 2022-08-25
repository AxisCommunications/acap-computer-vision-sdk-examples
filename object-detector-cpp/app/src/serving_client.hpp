/**
 * Copyright (C) 2020 Axis Communications AB, Lund, Sweden
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

#include <climits>
#include <iostream>
#include <math.h>
#include <memory>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <string>
#include <optional>

#include "google/protobuf/map.h"
#include "grpcpp/create_channel.h"
#include "grpcpp/security/credentials.h"

#include "tensorflow/core/framework/tensor.grpc.pb.h"
#include "tensorflow/core/framework/tensor_shape.grpc.pb.h"
#include "tensorflow/core/framework/types.grpc.pb.h"
#include "tensorflow_serving/apis/predict.grpc.pb.h"
#include "tensorflow_serving/apis/prediction_service.grpc.pb.h"

#define ZEROCOPY

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using tensorflow::serving::PredictionService;
using tensorflow::serving::PredictRequest;
using tensorflow::serving::PredictResponse;

using namespace std;

typedef google::protobuf::Map<std::string, tensorflow::TensorProto> OutMap;

/**
 * Inference serving client. Serving of inference over gRPC.
 */
class ServingClient {
public:
  /**
   * Class constructor.
   *
   * @param channel gRPC channel for requests
   */
  ServingClient(std::shared_ptr<Channel> channel)
      : stub_(PredictionService::NewStub(channel)) {
#ifdef ZEROCOPY
    // Create memory mapped file
    cerr << "Create memory mapped file" << endl;
    fd_ = shm_open(sharedFile_, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd_ < 0) {
      cerr << "Can not create memory mapped file" << endl;
    }
#endif
  }

  ~ServingClient() {
    if (fd_>= 0) {
      close(fd_);
    }
    shm_unlink(sharedFile_);
  }

  /**
   * Prediction request to inference server. Assembles the client's payload,
   * sends it and presents the response back from the server.
   *
   * @param model_name name of prediction model.
   * @param image to perform prediction on.
   * @return status message
   */
  optional<PredictResponse> callPredict(const std::string &model_name,
                                        const cv::Mat &image, string &output);

private:
  std::unique_ptr<PredictionService::Stub> stub_;
  int fd_ = -1;
  const char* sharedFile_ = "/capture.bmp";
};

optional<PredictResponse>
ServingClient::callPredict(const std::string &model_name, const cv::Mat &image,
                           string &output) {
  // Data we are sending to the server.
  PredictRequest request;
  request.mutable_model_spec()->set_name(model_name);

  google::protobuf::Map<std::string, tensorflow::TensorProto> &inputs =
      *request.mutable_inputs();

  tensorflow::TensorProto proto;

  proto.mutable_tensor_shape()->add_dim()->set_size(1);
  proto.mutable_tensor_shape()->add_dim()->set_size(image.rows);
  proto.mutable_tensor_shape()->add_dim()->set_size(image.cols);
  proto.mutable_tensor_shape()->add_dim()->set_size(image.channels());

  size_t size = image.rows * image.cols * image.channels();

  if (fd_ < 0) {
    proto.set_tensor_content(image.data, size);

    proto.set_dtype(tensorflow::DataType::DT_UINT8);

    inputs["data"] = proto;

    std::cout << "Waiting for response" << std::endl;

    PredictResponse response;
    ClientContext context;
    // The actual RPC.
    Status status = stub_->Predict(&context, request, &response);

    // Act upon its status.
    if (status.ok()) {
      output = "Call predict OK";
      return response;
    } else {
      output = "gRPC call return code: " + status.error_code() + string(": ") +
              status.error_message();
      return {};
    }
  }
  else
  {
    // Zero copy image buffer
    if (ftruncate(fd_, size) != 0) {
      output = "Can not set size of memory mapped file";
      return {};
    }

    // Get an address to fd's memory for this process's memory space
    void* data = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_, 0);
    if (data == MAP_FAILED) {
      output = "Can not create memory map";
      return {};
    }

    memcpy(data, image.data, size);
    proto.add_string_val(sharedFile_);
    proto.set_dtype(tensorflow::DataType::DT_STRING);
    inputs["data"] = proto;
    std::cout << "Waiting for response Z" << std::endl;

    PredictResponse response;
    ClientContext context;
    // The actual RPC.
    Status status = stub_->Predict(&context, request, &response);

    // Cleanup
    munmap(data, size);

    // Act upon its status.
    if (status.ok()) {
      output = "Call predict OK";
      return response;
    } else {
      output = "gRPC call return code: " + status.error_message();
      return {};
    }
  }
}
