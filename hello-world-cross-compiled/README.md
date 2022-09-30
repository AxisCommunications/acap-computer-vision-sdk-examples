*Copyright (C) 2021, Axis Communications AB, Lund, Sweden. All Rights Reserved.*

# A small, cross compiled C++ hello-world application

This example demonstrates how to create and run a simple, cross compiled C++ OpenCV hello-world application using the ACAP Computer Vision SDK.

## Example structure

Following are the list of files and a brief description of each file in the example:

```text
hello-world-cross-compiled
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
# For arm64
export ARCH=aarch64
```

### Set your camera IP address define APP name and clear Docker memory

```sh
# Set camera IP
export AXIS_TARGET_IP=<actual camera IP address>
export DOCKER_PORT=2376

# Define APP name
export APP_NAME=hello-world-cross-compiled

# Clean docker memory
docker --tlsverify -H tcp://$AXIS_TARGET_IP:$DOCKER_PORT system prune -af
```

### Build and run the images

With the environment setup, the `hello-world-cross-compiled` image can be built. The environment variables are supplied as build arguments such that they are made available to docker during the build process:

```sh
docker build . -t $APP_NAME --build-arg ARCH
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
