*Copyright (C) 2022, Axis Communications AB, Lund, Sweden. All Rights Reserved.*


# A minimal, machine learning inference application
This example demonstrates how to create a simple Python application that performs inference on the camera image. This is done using the ACAP Computer Vision SDK.

The example largely consists of three parts: the Python script [simple_inference.py](simple_inference.py) that is run on device, the [Dockerfile](Dockerfile) which build the application image and the [docker-compose.yml](docker-compose.yml) which specifies how the inference server and client is connected as well as specify the required mounts and environment variables needed.

## Example structure
Following are the list of files and a brief description of each file in the example
```bash
minimal-ml-inference
├── config
│   ├── env.aarch64
│   └── env.armv7hf
├── simple_inference.py
├── Dockerfile
├── Dockerfile.model
├── docker-compose.yml
└── README.md
```

* **config/*** - Environment configuration files
* **simple_inference.py** - A Python script that captures an image and send an inference call to the model server
* **Dockerfile** - Build instructions for the application image that is run on the camera
* **Dockerfile.model** - Build instructions for the inference model
* **docker-compose.yml** - A docker-compose file that specifies how the application is run (mounts, environment variables, etc.,)

## Requirements
To ensure compatibility with the examples, the following requirements shall be met:
* Camera: ARTPEC-{7-8} DLPU devices (e.g., Q1615 MkIII)
* docker-compose version 1.29 or higher
* Docker version 20.10.8 or higher
* Firmware: 10.9
* docker-acap installed on the camera
* docker-acap set to use TLS and external memory card

## How to run the code
The first thing to do is to setup the environment. Generally, the variables described here can mostly be set to the default value, i.e., as seen below. However, the `AXIS_TARGET_IP` needs to be changed to your device's IP.

```sh
# Set your camera IP address and clear docker memory
export AXIS_TARGET_IP=<actual camera IP address>
export DOCKER_PORT=2376
docker --tlsverify -H tcp://$AXIS_TARGET_IP:$DOCKER_PORT system prune -af
```

### Export environment variables for arm32 cameras
```sh
# Set environment variables
# ARCH defines what architecture to use (e.g., armv7hf, aarch64)
# RUNTIME_IMAGE defines what base image should be used for the application image 
# INFERENCE_SERVER is the image of the model server
# MODEL_NAME is the image holding the inference model
export ARCH=armv7hf
export RUNTIME_IMAGE=arm32v7/ubuntu:20.04
export INFERENCE_SERVER=axisecp/acap-runtime:0.6-armv7hf
export APP_NAME=minimal-ml-inference
export MODEL_NAME=acap-dl-models
export MODEL_IMAGE=arm32v7/alpine
```

### Export environment variables for arm64 cameras
```sh
export ARCH=aarch64
export RUNTIME_IMAGE=arm64v8/ubuntu:20.04
export INFERENCE_SERVER=axisecp/acap-runtime:0.6-aarch64
export APP_NAME=minimal-ml-inference
export MODEL_NAME=acap-dl-models
export MODEL_IMAGE=arm64v8/alpine
```

With the environment setup, the `minimal-ml-inference` image and inference models can be built:

```sh
docker build . -t $APP_NAME --build-arg ARCH --build-arg RUNTIME_IMAGE
docker build . -f Dockerfile.model -t $MODEL_NAME --build-arg MODEL_IMAGE
```

Next, the build images needs to be uploaded to the device. This can be done through a registry or directly. In this case, the direct transfer is used by piping the compressed application directly to the device's Docker client:

```sh
docker save $APP_NAME | docker --tlsverify -H tcp://$AXIS_TARGET_IP:$DOCKER_PORT load
docker save $MODEL_NAME | docker --tlsverify -H tcp://$AXIS_TARGET_IP:$DOCKER_PORT load
```

The following command pulls the inference server image from Docker Hub and starts the application. The environment variables are supplied as build arguments to the `docker-compose` command such that they are made available to Docker during the build process. As the example uses OpenCV, the OpenCV requirements will be included in `docker-compose.yml`, which is used to run the application:

```sh
docker-compose --tlsverify -H tcp://$AXIS_TARGET_IP:$DOCKER_PORT --env-file ./config/env.$ARCH up

# Terminate with ctrl-C and cleanup
docker-compose --tlsverify -H tcp://$AXIS_TARGET_IP:$DOCKER_PORT down -v
```

The expected output from the application is the raw predictions from the model specified in the environment variable.

## License
**[Apache License 2.0](../LICENSE)**
