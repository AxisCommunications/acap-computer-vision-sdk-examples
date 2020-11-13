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
 | |- Dockerfile - Builds the program using an image that contains everything necessary for building, and copies it into another image.
 | |- Makefile - Used by the make tool to build the program
 | |- build.sh - Builds and tags the image of objdetect.cpp image
 |- README.md - How to execute the example
 |- docker-compose.yml - Specifies the group of images used to run the application, and their interdependencies.
```

## Prerequisites
To get started following system requirements shall be met:
* Docker version 19.03.5 or higher
* Debian Stretch or Ubuntu 18.04
* Firmware: Q1615-MkIII_10.2.0_fimage.bin
* Docker Daemon installed on the camera
* Dockerhub ID to pull images (e.g., Inference-server, ssdlite_mobilenet_object etc.)


## How to run the code
The video tutorial shows the code executions steps below:
<div align="center">
      <a href="https://www.youtube.com/embed/K2NNb2XljV4">
         <img src="https://img.youtube.com/vi/K2NNb2XljV4/0.jpg">
      </a>
</div>

* To build the object-detector-cpp image.
```sh
# Find build folder 
cd acap-application-examples/object-detector-cpp/app

# Adjust some environment variables to your preference, then build and push to docker repo
export REPO=axisecp &&\
export VERSION=4.0-pre3 &&\
export ARCH=armv7hf &&\
export UBUNTU_VERSION=19.10 &&\
export BUILD_IMAGE=${REPO}/acap4-object-detector-cpp:1.1.1-api.${VERSION}-${ARCH}-ubuntu${UBUNTU_VERSION} &&\
./build.sh $BUILD_IMAGE &&\
docker push $BUILD_IMAGE
cd ..
```

* Use the following commands to run the example with images from Docker Hub:
```sh
# Set your camera IP address
export AXIS_TARGET_IP=<actual camera IP address>

# Clear docker memory on camera
docker -H tcp://$AXIS_TARGET_IP system prune -a

# Run on camera
docker-compose -H tcp://$AXIS_TARGET_IP:2375 up
```

### The expected output:
```
object-detector_1           | Caught frame 78 640x360
object-detector_1           | Connecting to: inference-server:8501
object-detector_1           | Waiting for response
object-detector_1           | Call predict OK
object-detector_1           | Object: car, Confidence: 0.58, Box: [0.71, 0.02, 0.98, 0.46]
object-detector_1           | Object: car, Confidence: 0.42, Box: [0.70, 0.16, 0.90, 0.45]
object-detector_1           | Object: car, Confidence: 0.42, Box: [0.75, 0.46, 0.79, 0.52]
object-detector_1           | Object: bicycle, Confidence: 0.34, Box: [0.77, 0.87, 0.89, 0.97]
object-detector_1           | Object: car, Confidence: 0.34, Box: [0.70, 0.01, 0.85, 0.23]
object-detector_1           | Object: car, Confidence: 0.34, Box: [0.70, 0.01, 0.75, 0.13]
object-detector_1           | Capture: 3 ms
object-detector_1           | Preprocess: 7 ms
object-detector_1           | Inference grpc call: 38 ms
object-detector_1           | Postprocess: 0 ms
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
This example uses larod-inference-server for video inference processing. The API uses an insecure gRPC communication channel, but it is possible to activate SSL/TLS server authentication and encryption by activating following define statement in file src/object_detect.cpp:
```c++
#define USE_SSL
```
When SSL/TLS is activated, a certificate and private key for your organization must be provided to the inference server. Here is an example how to generate a temporary test certificate:
```sh
# Generate TSL/SSL test certificate
# Press default for all input except: Common Name (e.g. server FQDN or YOUR name) []:localhost
 openssl req -x509 -newkey rsa:4096 -nodes -days 365 -out testdata/server.pem -keyout testdata/server.key
```
The inference server must be started by specifying the certificate and the private key in the file docker-compose.yml:
```sh
/usr/bin/larod-inference-server -c certificate.pem -k private.key
```
## Model over gRPC
This example uses larod-inference-server for video inference processing by using gRPC API. The inference server supports multiple clients at the same time. Models are normally loaded when the inference server is starting up, but models can also be loaded by specifying the model file path over gRPC. Please note the model path specified must be accessible by the inference server.

## License
**Apache License 2.0**

