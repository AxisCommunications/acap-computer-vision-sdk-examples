*Copyright (C) 2021, Axis Communications AB, Lund, Sweden. All Rights Reserved.*

# An object detection application in C++ on an edge device

This example code is written in C++ and implements an object detection scenario on the camera using Docker. The example uses the following technologies:

* OpenCV
* inference-server (larod)
* ssdlite-mobilenet-v2

## Overview

This example composes three different container images into an application that performs object detection using a deep learning model.

The first container contains the actual program built in this example. It uses [OpenCV](https://opencv.org/) to capture pictures from the camera and modifies them to fit the input required by the model. It then uses [gRPC](https://grpc.io/)/[protobuf](https://developers.google.com/protocol-buffers) to call the second container, the *inference-server*, that performs the actual inference by implementing the [TensorFlow Serving API](https://github.com/tensorflow/serving). You can find more documentation on the [Machine Learning API documentation page](https://axiscommunications.github.io/acap-documentation/docs/api/computer-vision-sdk-apis.html#machine-learning-api). This example uses a containerized version of the [ACAP Runtime](https://github.com/AxisCommunications/acap-runtime#containerized-version) as the *inference-server*.

Lastly, there is a third container that holds the deep learning model, which is put into a volume that is accessible by the other two images. The layout of the Docker image containing the model is shown below. The *MODEL_PATH* variable in the configuration file you're using specifies what model to use. By default, the armv7hf configuration file uses the edgetpu model, while the aarch64 configuration file uses the vanilla model.

```text
model
├── ssdlite-mobilenet-v2 - model for CPU
├── ssdlite-mobilenet-v2-tpu - model for TPU
└── objects.txt - list of object labels
```

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
  * [Docker ACAP](https://github.com/AxisCommunications/docker-acap#installing) installed and started, using TLS and SD card as storage
* Computer
  * Either [Docker Desktop](https://docs.docker.com/desktop/) version 4.11.1 or higher,
  * or [Docker Engine](https://docs.docker.com/engine/) version 20.10.17 or higher with BuildKit enabled using Docker Compose version 1.29.2 or higher

## How to run the code

### Export the environment variable for the architecture

Export the `ARCH` variable depending on the architecture of your camera:

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

### Build the Docker images

With the architecture defined, the `acap4-object-detector-cpp` and `acap-dl-models` images can be built. The environment variables are supplied as build arguments such that they are made available to Docker during the build process:

```sh
# Define app name
export APP_NAME=acap4-object-detector-cpp
export MODEL_NAME=acap-dl-models

# Install qemu to allow build flask for a different architecture
docker run --rm --privileged multiarch/qemu-user-static --credential yes --persistent yes

# Build app
docker build --tag $APP_NAME --build-arg ARCH .

# Build inference model
docker build --file Dockerfile.model --tag $MODEL_NAME --build-arg ARCH .
```

### Set your device IP address and clear Docker memory

```sh
DEVICE_IP=<actual camera IP address>
DOCKER_PORT=2376

docker --tlsverify --host tcp://$DEVICE_IP:$DOCKER_PORT system prune --all --force
```

If you encounter any TLS related issues, please see the TLS setup chapter regarding the `DOCKER_CERT_PATH` environment variable in the [Docker ACAP repository](https://github.com/AxisCommunications/docker-acap#securing-the-docker-acap-using-tls).

### Install the images

Next, the built images needs to be uploaded to the device. This can be done through a registry or directly. In this case, the direct transfer is used by piping the compressed application directly to the device's Docker client:

```sh
docker save $APP_NAME | docker --tlsverify --host tcp://$DEVICE_IP:$DOCKER_PORT load

docker save $MODEL_NAME | docker --tlsverify --host tcp://$DEVICE_IP:$DOCKER_PORT load
```

> [!NOTE]
> If the *inference-server* ([containerized ACAP Runtime](https://github.com/AxisCommunications/acap-runtime#containerized-version)) is not already present on the device, it will be pulled from Docker Hub
> when running `docker compose up`.
> If the pull action fails due to network connectivity, pull the image to your host system and load it to
> the device instead.

### Run the containers

With the images on the device, they can be started using `docker-compose.yml`:

```sh
docker --tlsverify --host tcp://$DEVICE_IP:$DOCKER_PORT compose --env-file ./config/env.$ARCH.$CHIP up

# Terminate with Ctrl-C and cleanup
docker --tlsverify --host tcp://$DEVICE_IP:$DOCKER_PORT compose --env-file ./config/env.$ARCH.$CHIP down --volumes
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

This example uses the inference server for video inference processing by using gRPC API. In case this client and the inference server is located on the same camera, it is possible to speed up inference by using shared memory to pass the video image to the inference server by activating following define statement in file `app/src/serving_client.hpp`:

```c++
#define ZEROCOPY
```

## Server authentication

This example uses the inference server for video inference processing. The API uses SSL/TLS server authentication and encryption as the default behavior, but can use an insecure gRPC communication channel if no server certificate is provided. For the secure communication, we need:

1. To provide a server certificate `server.pem` as the first parameter to object detector.
2. To start the inference server by specifying the server certificate `server.pem` and the private key `server.key`.

Both of these are present in the `docker-compose.yml` file.

## Model over gRPC

This example uses the inference server for video inference processing by using gRPC API. The inference server supports multiple clients at the same time. Models are normally loaded when the inference server is starting up, but models can also be loaded by specifying the model file path over gRPC. Please note the model path specified must be accessible by the inference server.

### Hardware acceleration

The `./config` folder contains configuration files with the parameters to run the inference on different camera models, also giving the possibility to use the hardware accelerator. To achieve the best performance we recommend using the TPU (Tensor Processing Unit) equipped with ARTPEC-7 cameras (e.g. [Axis-Q1615 Mk III](https://www.axis.com/products/axis-q1615-mk-iii)) or the DLPU (Deep Learning Processing Unit) equipped in ARTPEC-8 cameras (e.g. [Axis-Q1656](https://www.axis.com/products/axis-q1656)).

## License

**[Apache License 2.0](../LICENSE)**
