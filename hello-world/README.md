*Copyright (C) 2021, Axis Communications AB, Lund, Sweden. All Rights Reserved.*


# A minimal Python application
This example demonstrates how to create a simple Python application using the ACAP Computer Vision SDK and run it on an edge device.

Going from zero to a Python application running on an AXIS device is quite easy. First, the application script is written, as in the hello-world script in [app/simply_hello.py](app/simply_hello.py). Next, the [Dockerfile](Dockerfile) which build the application image is constructed. This needs to pull in packages from the ACAP Computer Vision SDK, as is done using the `COPY` commands. Finally, the application needs to be built and uploaded, as is specified below.

## Example Structure
Following are the list of files and a brief description of each file in the example
```bash
hello-world
├── app
├── |- simply_hello.py
├── Dockerfile
└── README.md
```

* **simply_hello.py** - A Python script that prints "Hello World"
* **Dockerfile** - Build instructions for the application image that is run on the camera

## Requirements
To ensure compatibility with the examples, the following requirements shall be met:
* Camera: ARTPEC-7 DLPU devices (e.g., Q1615 MkIII)
* docker-compose version 1.29 or higher
* Docker version 20.10.8 or higher
* Firmware: 10.7
* docker-acap installed on the camera
* docker-acap set to use TLS and external memory card

## How to run the code
The first thing to do is to setup the environment. Generally, the variables described here can mostly be set to the default value, i.e., as seen below. However, the `AXIS_TARGET_IP` needs to be changed to your device's IP.

```sh
# Set your camera IP address and clear docker memory
export AXIS_TARGET_IP=<actual camera IP address>
docker --tlsverify -H tcp://$AXIS_TARGET_IP:2376 system prune -af

# Set environment variables
# REPO defines where to get the ACAP Computer Vision SDK
# ARCH defines what architecture to use (e.g., armv7hf, aarch64)
# RUNTIME_IMAGE defines what base image should be used for the application image
export REPO=axisecp
export ARCH=armv7hf
export RUNTIME_IMAGE=arm32v7/ubuntu:20.04
export APP_NAME=hello-world
```

With the environment setup, the `hello-world` image can be built. The environment variables are supplied as build arguments such that they are made available to docker during the build process:

```sh
docker build -t $APP_NAME --build-arg REPO --build-arg ARCH --build-arg RUNTIME_IMAGE .
```

Next, the built image needs to be uploaded to the device. This can be done through a registry or directly. In this case, the direct transfer is used by piping the compressed application directly to the device's docker client:

```sh
docker save $APP_NAME | docker --tlsverify -H tcp://$AXIS_TARGET_IP:2376 load
```

With the application image on the device, it can be started. As this example does not use e.g., OpenCV, no special mounts are needed, making the run command very simple:

```sh
docker --tlsverify -H tcp://$AXIS_TARGET_IP:2376 run -it $APP_NAME
```

The expected output from the application is simply:

```sh
Hello World!
```

## License
**[Apache License 2.0](../LICENSE)**
