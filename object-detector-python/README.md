*Copyright (C) 2021, Axis Communications AB, Lund, Sweden. All Rights Reserved.*

# An inference example application on an edge device
This example is written in Python and implements the following object detection scenarios:
 - Run a video streaming inference on camera
 - Run a still image inference on camera

## Overview
This example composes three different container images into an application that performs object detection using a deep learning model.

The first container contains the actual program being built here, which uses OpenCV to capture pictures from the camera and modifies them to fit the input required by the model. It then uses grpc/protobuf to call the second container, the "inference-server", that performs the actual inference. The inference server implements the TensorFlow Serving API.

Lastly, there is a third container that holds the deep learning model, which is put into a volume that is accessible to the other two images.

**larod-inference server**\
Uses larod service in the camera firmware with model in docker image
`axisecp/acap-dl-models:ssdlite-mobilenet-v2`

The docker image containing the model has a layout as shown below. What model to use is specified by path in the docker-compose file.
```bash
model
├── ssdlite-mobilenet-v2 - model for CPU
├── ssdlite-mobilenet-v2-tpu - model for TPU
└── objects.txt - list of object labels
```

## Example structure
Following are the list of files and a brief description of each file in the example
```bash
object-detector-python
├── app
├── |- detector.py
├── |- dog416.png
├── docker-compose.yml
├── static-image.yml
├── Dockerfile
├── Dockerfile.model
└── README.md
```

* **detector.py** - The inference client main program
* **dog416.png** - Static image used with static-image.yml
* **docker-compose.yml** - Docker compose file for streaming camera video example using larod inference service
* **static-image.yml** - Docker compose file for static image debug example using larod inference service
* **Dockerfile** - Build Docker image with inference client for camera
* **Dockerfile.model** - Build Docker image with inference model

## Requirements
To ensure compatibility with the examples, the following requirements shall be met:
* Camera: ARTPEC-{7-8} DLPU devices (e.g., Q1615 MkIII)
* docker-compose version 1.29 or higher
* Docker version 20.10.8 or higher
* Firmware: 10.9
* [Docker ACAP](https://github.com/AxisCommunications/docker-acap) installed and started, using TLS and SD card as storage

## How to run the code
### Export environment variables for arm32 cameras
```sh
# Set your camera IP address and clear docker memory
export AXIS_TARGET_IP=<actual camera IP address>
export DOCKER_PORT=2376
docker --tlsverify -H tcp://$AXIS_TARGET_IP:$DOCKER_PORT system prune -af

# Set environment variables
export ARCH=armv7hf
export RUNTIME_IMAGE=arm32v7/ubuntu:20.04
export APP_NAME=acap4-object-detector-python
export MODEL_NAME=acap-dl-models
export MODEL_IMAGE=arm32v7/alpine
```

### Export environment variables for arm64 cameras
```sh
# Set your camera IP address and clear docker memory
export AXIS_TARGET_IP=<actual camera IP address>
export DOCKER_PORT=2376
docker --tlsverify -H tcp://$AXIS_TARGET_IP:$DOCKER_PORT system prune -af

# Set environment variables
export ARCH=aarch64
export RUNTIME_IMAGE=arm64v8/ubuntu:20.04
export APP_NAME=acap4-object-detector-python
export MODEL_NAME=acap-dl-models
export MODEL_IMAGE=arm64v8/alpine
```

### Build the object-detector-python image
```sh
# Build and upload object detector
docker build . -t $APP_NAME --build-arg ARCH --build-arg RUNTIME_IMAGE
docker save $APP_NAME | docker --tlsverify -H tcp://$AXIS_TARGET_IP:$DOCKER_PORT load

# Build and upload inference models
docker build . -f Dockerfile.model -t $MODEL_NAME --build-arg MODEL_IMAGE
docker save $MODEL_NAME | docker --tlsverify -H tcp://$AXIS_TARGET_IP:$DOCKER_PORT load

# Use the following command to run the example on the camera
docker-compose --tlsverify -H tcp://$AXIS_TARGET_IP:$DOCKER_PORT --env-file ./config/env.$ARCH  up

# Terminate with ctrl-C and cleanup
docker-compose --tlsverify -H tcp://$AXIS_TARGET_IP:$DOCKER_PORT down -v
```

### The expected output:
```sh
`docker-compose --tlsverify  -H tcp://$AXIS_TARGET_IP:2376 up`
```
....
object-detector_1           | 1 Objects found
object-detector_1           | person
```

`docker-compose --tlsverify  -H tcp://$AXIS_TARGET_IP:2376 -f static-image.yml up`
```
....
object-detector-python_1          | 3 Objects found
object-detector-python_1          | bicycle
object-detector-python_1          | dog
object-detector-python_1          | car
```

## License
**[Apache License 2.0](../LICENSE)**
