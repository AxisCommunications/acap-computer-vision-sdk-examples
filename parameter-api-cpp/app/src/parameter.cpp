/**
 * Copyright (C) 2022 Axis Communications AB, Lund, Sweden
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

#include "keyvaluestore.grpc.pb.h"
#include "grpcpp/create_channel.h"
#include "grpcpp/security/credentials.h"
#include "google/protobuf/map.h"
#include <sstream>
#include <sys/stat.h>

using namespace std;
using namespace keyvaluestore;
using namespace grpc;

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

const char* target = "unix:///tmp/acap-runtime.sock";

class Parameter {
  public:
    Parameter(std::shared_ptr<Channel> channel)
      : stub_(KeyValueStore::NewStub(channel)) {}

    // Requests each key in the vector and return the key
    // and its corresponding value as a pair
    vector<pair<string, string>> GetValues(
      const vector<string>& keys) {
        
      vector<pair<string,string>> values;
      ClientContext context;
      Request request;
      Response response;

      auto stream = stub_->GetValues(&context);     
      for (const auto& key : keys) {
        // Key we are sending to the server.
        request.set_key(key);
        stream->Write(request);

        // Get the value for the sent key
        if (stream->Read(&response)) {
          values.push_back(make_pair(key, response.value()));
        }
      }

      stream->WritesDone();
      Status status = stream->Finish();

      // Act upon its status.
      if (!status.ok()) {
        throw std::runtime_error("Can not access gRPC channel: " + status.error_message());        
      }
      return values;
    }  
 private:
  std::unique_ptr<KeyValueStore::Stub> stub_;
};

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

// main function
int main(int argc, char* argv[])
{ 
  // camera keys
  vector<string> keys = {
    "root.Brand.Brand",
    "root.Brand.WebURL",
    "root.Image.I0.Enabled",
    "root.Brand.ProdFullName",
    "root.Brand.ProdNbr",
    "root.invalid"
  };

  try {
    // Create channel
    shared_ptr<Channel> channel;     
    if (argc > 1) {
      // gRPC secure connection
      string root_cert = read_text(argv[1]);
      SslCredentialsOptions ssl_opts = {root_cert.c_str(), "", ""};
      shared_ptr<ChannelCredentials> creds = grpc::SslCredentials(ssl_opts);
      grpc::ChannelArguments args;
      args.SetSslTargetNameOverride("localhost");
      channel = grpc::CreateCustomChannel(target, creds, args);
    }
    else {
      // gRPC insecure connection
      channel = grpc::CreateChannel(
        target, grpc::InsecureChannelCredentials());
    }

    // Get parameters
    Parameter param(channel);
    vector<pair<string,string>> values = param.GetValues(keys);
    // Display camera key and value pair
    for (pair<string,string> value: values) {
      cout << value.first << " : " << value.second << endl;
    }
    return 0;
  }
  catch(const std::exception& e)
  {
    cerr << "Error has occurred: " << e.what() << endl;
  }
}