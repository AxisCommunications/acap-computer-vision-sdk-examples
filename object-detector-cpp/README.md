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

## Example structure

Below is the structure of the example with a brief description of scripts.

```text
object-detector-cpp
 |- app
 | |- src
 | | |- object_detect.cpp - App to capture the video stream using OpenCV in C++ to detect objects.
 | | |- serving_client.hpp - Creates the request and makes the call to the inference-server.
 | | |- test_certificate.h - Certificate for accessing the inference-server using SSL/TLS.
 | |- Makefile - Used by the make tool to build the program
 |- config
 | |- .env.aarch64 - Configuration file to run docker-compose on an arm64 device
 | |- .env.armv7hf - Configuration file to run docker-compose on an arm32 device
 |- docker-compose.yml - Specifies the group of images used to run the application, and their interdependencies.
 |- Dockerfile - Specifies how the application is built.
 |- Dockerfile.model - Specifies how the inference model is built.
 |- README.md - How to execute the example
```

## Requirements

Meet the following requirements to ensure compatibility with the example:

* Axis device
  * Chip: ARTPEC-{7-8} DLPU devices (e.g., Q1615 MkIII)
  * Firmware: 10.9 or higher
  * [Docker ACAP](https://github.com/AxisCommunications/docker-acap) installed and started, using TLS and SD card as storage
* Computer
  * Either [Docker Desktop](https://docs.docker.com/desktop/) version 4.11.1 or higher, or [Docker Engine](https://docs.docker.com/engine/) version 20.10.17 or higher with BuildKit enabled using Docker Compose version 1.29.2 or higher

## How to run the code

### Export the environment variable for the architecture

Export the ARCH variable depending on the architecture of your camera

```sh
# For arm32
export ARCH=armv7hf
# Valid options for chip on armv7hf are 'tpu' (hardware accelerator) or 'cpu'
export CHIP=tpu
```

```sh
# For arm64
export ARCH=aarch64
# Valid options for chip on aarch64 are 'artpec8' (hardware accelerator) or 'cpu'
export CHIP=artpec8
```

### Set your camera IP address and clear Docker memory

```sh
export AXIS_TARGET_IP=<actual camera IP address>
export DOCKER_PORT=2376
docker --tlsverify -H tcp://$AXIS_TARGET_IP:$DOCKER_PORT system prune -af
```

### Build the object-detector-cpp images

```sh
# Define APP name
export APP_NAME=acap4-object-detector-cpp
export MODEL_NAME=acap-dl-models

# Install qemu to allow build flask for a different architecture
docker run -it --rm --privileged multiarch/qemu-user-static --credential yes --persistent yes

# Build and upload inference client for camera
docker build . -t $APP_NAME --build-arg ARCH
docker save $APP_NAME | docker --tlsverify -H tcp://$AXIS_TARGET_IP:$DOCKER_PORT load

# Build and upload inference models
docker build . -f Dockerfile.model -t $MODEL_NAME --build-arg ARCH
docker save $MODEL_NAME | docker --tlsverify -H tcp://$AXIS_TARGET_IP:$DOCKER_PORT load

# Use the following command to run the example on the camera
docker-compose --tlsverify -H tcp://$AXIS_TARGET_IP:$DOCKER_PORT --env-file ./config/env.$ARCH.$CHIP up

# Terminate with Ctrl-C and cleanup
docker-compose --tlsverify -H tcp://$AXIS_TARGET_IP:$DOCKER_PORT --env-file ./config/env.$ARCH.$CHIP down -v
```

### The expected output

```text
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

Depending on the network, you might need proxy settings in the following file: `~/.docker/config.json`.

For reference please see: https://docs.docker.com/network/proxy/.

## Zero copy

This example uses larod-inference-server for video inference processing by using gRPC API. In case this client and the inference server is located on the same camera, it is possible to speed up inference by using shared memory to pass the video image to the inference server by activating following define statement in file src/serving_client.hpp:

```c++
#define ZEROCOPY
```

## Server authentication

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

### Hardware acceleration

The ./config folder contains configuration files with the parameters to run the inference on different camera models, also giving the possibility to use the hardware accelerator.
To achieve the best performance we recommend using the TPU (Tensor Processing Unit) equipped with artpec7 cameras (e.g. [Axis-Q1615 Mk III](https://www.axis.com/products/axis-q1615-mk-iii))
or the DLPU (Deep Learning Processing Unit) equipped in artpec8 cameras (e.g. [Axis-Q1656](https://www.axis.com/products/axis-q1656))

## License

**[Apache License 2.0](../LICENSE)**
