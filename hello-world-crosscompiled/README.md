*Copyright (C) 2021, Axis Communications AB, Lund, Sweden. All Rights Reserved.*


# A small, crosscompiled C++ hello-world application
This example demonstrates how to create and run a simple, crosscompiled C++ OpenCV hello-world application using the ACAP Computer Vision SDK.

## Example structure
Following are the list of files and a brief description of each file in the example:
```bash
hello-world-crosscompiled
├── app
│   ├── Makefile
│   └── src
│       └── hello_world.cpp
├── docker-compose.yml
└── Dockerfile
```

* **hello_world.cpp** - A Hello World application which writes to standard out.
* **docker-compose.yml** - A docker-compose file that specifies how the application is run (mounts, environment variables, etc.,)
* **Dockerfile** - Build instructions for the application image that is run on the camera
* **Makefile** - Makefile containing the build and link instructions for building the ACAP application.

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


export ARCH=armv7hf
export RUNTIME_IMAGE=arm32v7/ubuntu:20.04
export APP_NAME=hello-world-crosscompiled
```
### Export environment variables for arm64 cameras
```sh

export ARCH=aarch64
export RUNTIME_IMAGE=arm64v8/ubuntu:20.04
export APP_NAME=hello-world-crosscompiled
```

With the environment setup, the `hello-world-crosscompiled` image can be built. The environment variables are supplied as build arguments such that they are made available to docker during the build process:

```sh
docker build . -t $APP_NAME --build-arg ARCH --build-arg RUNTIME_IMAGE
```

Next, the built image needs to be uploaded to the device. This can be done through a registry or directly. In this case, the direct transfer is used by piping the compressed application directly to the device's docker client:

```sh
docker save $APP_NAME | docker --tlsverify -H tcp://$AXIS_TARGET_IP:$DOCKER_PORT  load
```

With the application image on the device, it can be started. As the example uses OpenCV, the OpenCV requirements will be included in `docker-compose.yml`, which is used to run the application:

```sh
docker-compose --tlsverify -H tcp://$AXIS_TARGET_IP:$DOCKER_PORT up

# Cleanup after execution
docker-compose --tlsverify -H tcp://$AXIS_TARGET_IP:$DOCKER_PORT down -v
```

The expected output from the application is (depending on the OpenCV pulled from the ACAP Computer Vision SDK):

```sh
...
Hello World from OpenCV 4.5.1
```
## License
**[Apache License 2.0](../LICENSE)**
