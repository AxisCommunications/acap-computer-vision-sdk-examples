*Copyright (C) 2021, Axis Communications AB, Lund, Sweden. All Rights Reserved.*

# An inference example application on an edge device

This example is written in Python and implements the following object detection scenarios:

* Run a video streaming inference on camera
* Run a still image inference on camera

## Overview

This example composes three different container images into an application that performs object detection using a deep learning model.

The first container contains the actual program being built here, which uses OpenCV to capture pictures from the camera and modifies them to fit the input required by the model. It then uses grpc/protobuf to call the second container, the "inference-server", that performs the actual inference. The inference server implements the TensorFlow Serving API.

Lastly, there is a third container that holds the deep learning model, which is put into a volume that is accessible to the other two images.

**larod-inference server**\
Uses larod service in the camera firmware with model in docker image
`axisecp/acap-dl-models:ssdlite-mobilenet-v2`

The docker image containing the model has a layout as shown below. What model to use is specified by path in the docker-compose file.

```text
model
├── ssdlite-mobilenet-v2 - model for CPU
├── ssdlite-mobilenet-v2-tpu - model for TPU
└── objects.txt - list of object labels
```

## Example structure

Following are the list of files and a brief description of each file in the example

```text
object-detector-python
├── app
│   ├── detector.py
│   └── dog416.png
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

Meet the following requirements to ensure compatibility with the example:

* Axis device
  * Chip: ARTPEC-{7-8} DLPU devices (e.g., Q1615 MkIII)
  * Firmware: 10.9 or higher
  * [Docker ACAP](https://github.com/AxisCommunications/docker-acap) installed and started, using TLS and SD card as storage
* Computer
  * Either [Docker Desktop](https://docs.docker.com/desktop/) version 4.11.1 or higher, or [Docker Engine](https://docs.docker.com/engine/) version 20.10.17 or higher with BuildKit enabled using Docker Compose version 1.29.2 or higher

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

With the architecture defined, the `acap4-object-detector-python` and `acap-dl-models` images can be built. The environment variables are supplied as build arguments such that they are made available to docker during the build process:

```sh
# Define app name
export APP_NAME=acap4-object-detector-python
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

If you encounter any TLS related issues, please see the TLS setup chapter regarding the `DOCKER_CERT_PATH` environment variable in the [Docker ACAP repository](https://github.com/AxisCommunications/docker-acap).

### Install the images

Next, the built images needs to be uploaded to the device. This can be done through a registry or directly. In this case, the direct transfer is used by piping the compressed application directly to the device's docker client:

```sh
docker save $APP_NAME | docker --tlsverify --host tcp://$DEVICE_IP:$DOCKER_PORT load

docker save $MODEL_NAME | docker --tlsverify --host tcp://$DEVICE_IP:$DOCKER_PORT load
```

### Run the containers

With the application image on the device, it can be started using `docker-compose.yml`:

```sh
docker-compose --tlsverify --host tcp://$DEVICE_IP:$DOCKER_PORT --env-file ./config/env.$ARCH.$CHIP up

# Terminate with Ctrl-C and cleanup
docker-compose --tlsverify --host tcp://$DEVICE_IP:$DOCKER_PORT --env-file ./config/env.$ARCH.$CHIP down --volumes
```

### The expected output

```sh
docker-compose --tlsverify --host tcp://$DEVICE_IP:$DOCKER_PORT --env-file ./config/env.$ARCH.$CHIP up
....
object-detector_1           | 1 Objects found
object-detector_1           | person
```

```sh
docker-compose --tlsverify --host tcp://$DEVICE_IP:$DOCKER_PORT --file static-image.yml --env-file ./config/env.$ARCH.$CHIP up
....
object-detector-python_1          | 3 Objects found
object-detector-python_1          | bicycle
object-detector-python_1          | dog
object-detector-python_1          | car
```

### Hardware acceleration

The `./config` folder contains configuration files with the parameters to run the inference on different camera models, also giving the possibility to use the hardware accelerator. To achieve the best performance we recommend using the TPU (Tensor Processing Unit) equipped with ARTPEC-7 cameras (e.g. [Axis-Q1615 Mk III](https://www.axis.com/products/axis-q1615-mk-iii)) or the DLPU (Deep Learning Processing Unit) equipped in ARTPEC-8 cameras (e.g. [Axis-Q1656](https://www.axis.com/products/axis-q1656)).

## License

**[Apache License 2.0](../LICENSE)**
