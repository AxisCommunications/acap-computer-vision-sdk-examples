#include <climits>
#include <iostream>
#include <math.h>
#include <memory>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <string>

#include "google/protobuf/map.h"
#include "grpcpp/create_channel.h"
#include "grpcpp/security/credentials.h"

#include "tensorflow/core/framework/tensor.grpc.pb.h"
#include "tensorflow/core/framework/tensor_shape.grpc.pb.h"
#include "tensorflow/core/framework/types.grpc.pb.h"
#include "tensorflow_serving/apis/predict.grpc.pb.h"
#include "tensorflow_serving/apis/prediction_service.grpc.pb.h"

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
      : stub_(PredictionService::NewStub(channel)) {}

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

  size_t size = image.rows * image.cols * 3;
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
