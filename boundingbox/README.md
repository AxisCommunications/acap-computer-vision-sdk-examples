 *Copyright (C) 2020, Axis Communications AB, Lund, Sweden. All Rights Reserved.*

# A boundingbox based ACAP3 application on an edge device
This README file explains how to build an ACAP4 application using the boundingbox API. It is achieved by using the containerized Axis API and toolchain images.

Together with this README file, you should be able to find a directory called app. That directory contains the "boundingbox example" application source code which can easily be compiled and run with the help of the tools and step by step below.

The boundingbox library aims to be a easy-to-use high-performance library for drawing plain boxes.

This example illustrates how to draw boundingboxes on different video channels.

## Getting started
These instructions will guide you on how to execute the code. Below is the structure and scripts used in the example:

```bash
boundingbox
├── app
│   ├── bbox_example.cc
│   ├── LICENSE
│   └── Makefile
├── Dockerfile
├── docker-compose.yml
└── README.md
```

* **app/bbox_example.cc** - Application that draws boundingboxes in C++.
* **app/LICENSE** - Text file which lists all open source licensed source code distributed with the application.
* **app/Makefile** - Makefile containing the build and link instructions for building the ACAP3 application.
* **Dockerfile** - Docker file with the specified Axis toolchain and API container to build the example specified.
* **docker-compose.yml** - Docker compose file is used to run the example.
* **README.md** - Step by step instructions on how to build and run the example.

### Limitations
* The example is done for the armv7hf architecture, but it is possible to use with aarch64 architecture.

### Build the application

> [!IMPORTANT]
> *Depending on the network you are connected to,
The file that needs those settings is: *~/.docker/config.json.*
For reference please see: https://docs.docker.com/network/proxy/ and a
[script for Axis device here](../FAQs.md#HowcanIset-upnetworkproxysettingsontheAxisdevice?).*

Standing in your working directory run the following commands:
```sh
# Locate Boundingbox folder
cd acap-application-examples/boundingbox

# Adjust some environment variables to your preference, then build and push to docker repo
export REPO=axisecp
export VERSION=3.2
export ARCH=armv7hf
export UBUNTU_VERSION=19.10
export APP_IMAGE=docker-sandbox.se.axis.com/axisecp/boundingboxexample:1.0.0-sdk.${VERSION}-${ARCH}-ubuntu${UBUNTU_VERSION}
docker build . --tag $APP_IMAGE --build-arg REPO --build-arg VERSION --build-arg ARCH --build-arg UBUNTU_VERSION
docker push $APP_IMAGE
```

### Install your application
* Use the following commands to run the example with images from Docker repository:
```sh
# Set your camera IP address
export AXIS_TARGET_IP=<actual camera IP address>

# Clear docker memory on camera
docker -H tcp://$AXIS_TARGET_IP system prune -af

# Run on camera
docker -H tcp://$AXIS_TARGET_IP pull $APP_IMAGE
docker-compose -H tcp://$AXIS_TARGET_IP:2375 up
```

#### The expected output
The boundingboxes will be visible both in liveview and recordings from channel 1 and 2.

When example starts a bounding box is drawn on channel 1 for 10 seconds, then followed by bounding boxes drawn on channel 1 and 2 for another 10 seconds. After that a walking box pattern in different colors is drawn on channel 1. This sequence then repeats forever.

## License
**[Apache License 2.0](../LICENSE)**
