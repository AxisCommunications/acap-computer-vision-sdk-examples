*Copyright (C) 2022, Axis Communications AB, Lund, Sweden. All Rights Reserved.*

# A Beta version of Parameter-API example application in Python on an edge device

[![Build parameter-api-python application](https://github.com/AxisCommunications/acap-computer-vision-sdk-examples/actions/workflows/parameter-api-python.yml/badge.svg)](https://github.com/AxisCommunications/acap-computer-vision-sdk-examples/actions/workflows/parameter-api-python.yml)

The example is written in Python and communicates with the [ACAP Runtime](https://github.com/AxisCommunications/acap-runtime) using gRPC on a Unix Domain Socket (UDS). ACAP Runtime acts as a server that exposes the Parameter-API.

## Overview

A network device from Axis has a lot of parameters and in this example we will use the Parameter-API, a service in [ACAP Runtime](https://github.com/AxisCommunications/acap-runtime) to read the values of a set of parameters and print them to the application log. The Parameter-API can read parameters, but cannot read parameter groups nor set parameter values.It is necessary to use the exact parameter name to get the expected results. The parameter list can be found using the URL `http://<ip address>/axis-cgi/param.cgi?action=list` where `<ip address>` is the IP address of your device.

## Example structure

These are the files used in the example, followed by a brief description of each file:

```text
parameter-api-python
├── app
│   └── parameter.py
├── docker-compose.yml
├── Dockerfile
└── README.md
```

* **app/parameter.py** - App to retrieve Axis device parameters
* **docker-compose.yml** - Specifies the images used to run the application, and their interdependencies
* **Dockerfile** - Specifies how the application is built
* **README.md** - Instructions on how to build and run the application

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

Define and export the application image name in `APP_NAME` for use in the Docker Compose file.

```sh
export APP_NAME=parameter-api

# Install qemu to allow build for a different architecture
docker run --rm --privileged multiarch/qemu-user-static --credential yes --persistent yes

# Build application image
docker build --tag $APP_NAME .
```

### Set your device IP address and clear Docker memory

```sh
DEVICE_IP=<actual camera IP address>
DOCKER_PORT=2376

docker --tlsverify --host tcp://$DEVICE_IP:$DOCKER_PORT system prune --all --force
```

If you encounter any TLS related issues, please see the TLS setup chapter regarding the `DOCKER_CERT_PATH` environment variable in the [Docker ACAP repository](https://github.com/AxisCommunications/docker-acap#securing-the-docker-acap-using-tls).

### Install the image

The built image needs to be uploaded to the device. This can be done through a registry or directly. In this case, the direct transfer is used by piping the compressed application directly to the device's Docker client:

```sh
docker save $APP_NAME | docker --tlsverify --host tcp://$DEVICE_IP:$DOCKER_PORT load
```

> [!NOTE]
> If the *acap-runtime* ([containerized ACAP Runtime](https://github.com/AxisCommunications/acap-runtime)) is not already present on the device, it will be pulled from Docker Hub
> when running `docker compose up`.
> If the pull action fails due to network connectivity, pull the image to your host system and load it to
> the device instead.

### Run the container

With the application image on the device, it can be started using `docker-compose.yml`:

```sh
docker --tlsverify --host tcp://$DEVICE_IP:$DOCKER_PORT compose up

# Terminate with Ctrl-C and cleanup
docker --tlsverify --host tcp://$DEVICE_IP:$DOCKER_PORT compose down --volumes
```

### The expected output

```text
parameter-api-python-parameter-api-1  | root.Brand.Brand : AXIS
parameter-api-python-parameter-api-1  | root.Brand.WebURL : http://www.axis.com
parameter-api-python-parameter-api-1  | root.Image.I0.Enabled : yes
parameter-api-python-parameter-api-1  | root.Brand.ProdFullName : AXIS Q1615 Mk III Network Camera
parameter-api-python-parameter-api-1  | root.Brand.ProdNbr : Q1615 Mk III
```

## License

**[Apache License 2.0](../LICENSE)**
