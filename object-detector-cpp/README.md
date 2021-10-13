*Copyright (C) 2021, Axis Communications AB, Lund, Sweden. All Rights Reserved.*

# An object detection application in C++ on an edge device
The example code is written in C++ for object detection on the camera using docker. The example uses the following technologies.
* OpenCV
* Larod inference-server
* ssdlite-mobilenet-v2

## Overview
This example composes three different container images into an application that performs object detection using a deep learning model.

The first container contains the actual program being built here, which uses OpenCV to capture pictures from the camera and modifies them to fit the input required by the model. It then uses grpc/protobuf to call the second container, the "inference-server", that performs the actual inference. The inference server implements the TensorFlow Serving API.

Lastly, there is a third container that holds the deep learning model, which is put into a volume that is accessible to the other two images.

## Example Structure
Below is the structure of the example with a brief description of scripts.
```shell
object-detector-cpp
 |- app
 | |- src
 | | |- object_detect.cpp - App to capture the video stream using OpenCV in C++ to detect objects.
 | | |- serving_client.hpp - Creates the request and makes the call to the inference-server.
 | | |- test_certificate.h - Certificate for accessing the inference-server using SSL/TLS.
 | |- Makefile - Used by the make tool to build the program
 |- docker-compose.yml - Specifies the group of images used to run the application, and their interdependencies.
 |- Dockerfile - Specifies how the application is built.
 |- Dockerfile.model - Specifies how the inference model is built.
 |- README.md - How to execute the example
```

## Requirements
To ensure compatibility with the examples, the following requirements shall be met:
* Camera: ARTPEC-7 DLPU devices (e.g., Q1615 MkIII)
* docker-compose version 1.29 or higher
* Docker version 20.10.8 or higher
* Firmware: 10.7
* docker-acap installed on the camera
* docker-acap set to use external memory card

## How to run the code
### Build the object-detector-cpp image
```sh
# Adjust some environment variables to your preference, then build and push to docker repo
export REPO=axisecp
export ARCH=armv7hf
export RUNTIME_IMAGE=arm32v7/ubuntu:20.04

# To allow retrieval of the image from the cloud
# this should be a repository that you can push to
# and that your camera can pull from, i.e., substitute
# axisecp for your own repository
export APP_NAME=axisecp/acap4-object-detector-cpp

docker build . -t $APP_NAME --build-arg REPO --build-arg ARCH --build-arg RUNTIME_IMAGE
docker push $APP_NAME
```
* Build docker container with inference models:
```sh
# To allow retrieval of the image from the cloud
# this should be a repository that you can push to
# and that your camera can pull from, i.e., substitute
# axisecp for your own repository
export MODEL_NAME=axisecp/acap-dl-models:1.1
docker build . -f Dockerfile.model -t $MODEL_NAME
docker push $MODEL_NAME
```

* Use the following commands to run the example with images from Docker Hub:
```sh
# Set your camera IP address
export AXIS_TARGET_IP=<actual camera IP address>

# Clear docker memory on camera
docker -H tcp://$AXIS_TARGET_IP system prune -af

# Run on camera
docker-compose -H tcp://$AXIS_TARGET_IP:2375 up
```

### The expected output:
```
object-detector_1   | Caught frame  2 480x320
object-detector_1   | Connecting to: inference-server:8501
object-detector_1   | Waiting for response Z
object-detector_1   | Call predict OK
object-detector_1   | Object: 51  banana, Confidence: 0.91, Box: [-0.00, -0.00, 0.98, 0.93]
object-detector_1   | Capture: 90 ms
object-detector_1   | Inference grpc call: 35 ms
object-detector_1   | Postprocess: 0 ms
```
## Proxy settings
Depending on the network, you might need proxy settings in the following file: *~/.docker/config.json.*

For reference please see: https://docs.docker.com/network/proxy/.

*Proxy settings can also be added on the edge device*
```sh
  ssh root@<CAMERA_IP>
```
**Run on the device:**
```sh
  #!/bin/sh
  cat >> /etc/systemd/system/sdkrun_dockerd.service <<EOF
  [Service]
  Environment="HTTP_PROXY=http://<myproxy.com>:<port>"
  Environment="HTTPS_PROXY=http://<myproxy>:<port>"
  Environment="NO_PROXY=localhost,127.0.0.0/8,10.0.0.0/8,192.168.0.0/16,172.16.0.0/12,.<domain>"
  EOF

  systemctl daemon-reload
  systemctl restart sdkrun_dockerd
```
## Zero copy
This example uses larod-inference-server for video inference processing by using gRPC API. In case this client and the inference server is located on the same camera, it is possible to speed up inference by using shared memory to pass the video image to the inference server by activating following define statement in file src/serving_client.hpp:
```c++
#define ZEROCOPY
```
## Server Authentication
This example uses larod-inference-server for video inference processing. The API uses an insecure gRPC communication channel when no certificate is provided and it uses SSL/TLS server authentication and encryption when a server certificate is provided as the first parameter to object detector:

```sh
objdetector server.pem
```

The inference server must then be started by specifying the server certificate and the private key in the file docker-compose.yml:
```sh
larod-inference-server -c server.pem -k private.key
```
## Model over gRPC
This example uses larod-inference-server for video inference processing by using gRPC API. The inference server supports multiple clients at the same time. Models are normally loaded when the inference server is starting up, but models can also be loaded by specifying the model file path over gRPC. Please note the model path specified must be accessible by the inference server.

## License
**[Apache License 2.0](../LICENSE)**
