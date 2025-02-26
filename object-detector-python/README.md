*Copyright (C) 2021, Axis Communications AB, Lund, Sweden. All Rights Reserved.*

# An inference example application on an edge device

This example is written in Python and implements the following object detection scenarios:

* Run a video streaming inference on camera
* Run a still image inference on camera

## Overview

This example composes three different container images into an application that performs object detection using a deep learning model.

The first container contains the actual program built in this example. It uses [OpenCV](https://opencv.org/) to capture pictures from the camera and modifies them to fit the input required by the model. It then uses [gRPC](https://grpc.io/)/[protobuf](https://developers.google.com/protocol-buffers) to call the second container, the *inference-server*, that performs the actual inference by implementing the [TensorFlow Serving API](https://github.com/tensorflow/serving). You can find more documentation on the [Machine Learning API documentation page](https://developer.axis.com/acap/api/computer-vision-sdk-apis/#machine-learning-api). This example uses a containerized version of the [ACAP Runtime](https://github.com/AxisCommunications/acap-runtime#containerized-version) as the *inference-server*.

Lastly, there is a third container that holds the deep learning model, which is put into a volume that is accessible by the other two images. The layout of the Docker image containing the model is shown below. The *MODEL_PATH* variable in the configuration file you're using specifies what model to use.

```text
model
├── ssdlite-mobilenet-v2 - model for CPU
└── objects.txt - list of object labels
```

## Example structure

Following are the list of files and a brief description of each file in the example

```text
object-detector-python
├── app
│   ├── detector.py
│   └── dog416.png
├── config
│   ├── env.aarch64.artpec8
│   └── env.aarch64.cpu
├── docker-compose.yml
├── static-image.yml
├── Dockerfile
├── Dockerfile.model
└── README.md
```

* **config/*** - Environment configuration files
* **detector.py** - The inference client main program
* **dog416.png** - Static image used with static-image.yml
* **docker-compose.yml** - Docker compose file for streaming camera video example using larod inference server
* **static-image.yml** - Docker compose file for static image debug example using larod inference server
* **Dockerfile** - Build Docker image with inference client for camera
* **Dockerfile.model** - Build Docker image with inference model

## Requirements

Meet the following requirements to ensure compatibility with the example:

* Axis device
  * Chip: ARTPEC-8 DLPU devices (e.g., Q1656)
  * Firmware: 11.10 or higher
  * [Docker ACAP](https://github.com/AxisCommunications/docker-acap#installing) version 3.0 installed and started, using TLS with TCP and IPC socket and SD card as storage
* Computer
  * Either [Docker Desktop](https://docs.docker.com/desktop/) version 4.11.1 or higher,
  * or [Docker Engine](https://docs.docker.com/engine/) version 20.10.17 or higher with BuildKit enabled using Docker Compose version 1.29.2 or higher

## How to run the code

### Build the Docker images

Define and export the application image name in `APP_NAME` and the model image name in `MODEL_NAME` for use in the Docker Compose file.
Define and export also the `CHIP` parameter to be used during the build to select the right manifest file.

```sh
export APP_NAME=acap4-object-detector-python
export MODEL_NAME=acap-dl-models
export CHIP=artpec8 # Valid options are 'artpec8' (hardware accelerator) or 'cpu'

# Install qemu to allow build for a different architecture
docker run --rm --privileged multiarch/qemu-user-static --credential yes --persistent yes

# Build application image
docker build --tag $APP_NAME .

# Build inference model image
docker build --file Dockerfile.model --tag $MODEL_NAME .
```

### Set your device IP address and clear Docker memory

```sh
DEVICE_IP=<actual camera IP address>
DOCKER_PORT=2376

docker --tlsverify --host tcp://$DEVICE_IP:$DOCKER_PORT system prune --all --force
```

If you encounter any TLS related issues, please see the TLS setup chapter regarding the `DOCKER_CERT_PATH` environment variable in the [Docker ACAP repository](https://github.com/AxisCommunications/docker-acap#securing-the-docker-acap-using-tls).

### Install the images

Browse to the application page of the Axis device:

```sh
http://<AXIS_DEVICE_IP>/index.html#apps
```

Click on the tab `Apps` in the device GUI and enable `Allow unsigned apps` toggle.

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

With the application image on the device, it can be started using `docker-compose.yml`:

```sh
docker --tlsverify --host tcp://$DEVICE_IP:$DOCKER_PORT compose --env-file ./config/env.aarch64.$CHIP up

# Terminate with Ctrl-C and cleanup
docker --tlsverify --host tcp://$DEVICE_IP:$DOCKER_PORT compose --env-file ./config/env.aarch64.$CHIP down --volumes
```

### The expected output

```sh
docker --tlsverify --host tcp://$DEVICE_IP:$DOCKER_PORT --env-file ./config/env.aarch64.$CHIP compose up
....
object-detector_1           | 1 Objects found
object-detector_1           | person
```

```sh
docker --tlsverify --host tcp://$DEVICE_IP:$DOCKER_PORT --file static-image.yml --env-file ./config/env.aarch64.$CHIP compose up
....
object-detector-python_1          | 3 Objects found
object-detector-python_1          | bicycle
object-detector-python_1          | dog
object-detector-python_1          | car
```

### Hardware acceleration

The `./config` folder contains configuration files with the parameters to run the inference on different camera models, also giving the possibility to use the hardware accelerator.
To achieve the best performance we recommend using DLPU (Deep Learning Processing Unit) equipped ARTPEC-8 cameras. See [ACAP Computer Vision SDK hardware and compatibility](https://developer.axis.com/acap/axis-devices-and-compatibility/#acap-computer-vision-solution-compatibility)

## License

**[Apache License 2.0](../LICENSE)**
