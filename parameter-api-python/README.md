*Copyright (C) 2022, Axis Communications AB, Lund, Sweden. All Rights Reserved.*

# A Beta version of Parameter-API example application in Python on an edge device

[![Build parameter-api-python application](https://github.com/AxisCommunications/acap-computer-vision-sdk-examples/actions/workflows/parameter-api-python.yml/badge.svg)](https://github.com/AxisCommunications/acap-computer-vision-sdk-examples/actions/workflows/parameter-api-python.yml)

The example is written in Python and communicates with the [ACAP Runtime](https://github.com/AxisCommunications/acap-runtime#native-acap-application) using gRPC on a Unix Domain Socket (UDS). ACAP Runtime acts as a server that exposes the Parameter-API.

## Overview

A network device from Axis has a lot of parameters and in this example we will use the Parameter-API, a service in [ACAP Runtime](https://github.com/AxisCommunications/acap-runtime) to read the values of a set of parameters and print them to the application log. The Parameter-API can read parameters, but cannot read parameter groups nor set parameter values.It is necessary to use the exact parameter name to get the expected results. The parameter list can be found using the URL `http://<ip address>/axis-cgi/param.cgi?action=list` where `<ip address>` is the IP address of your device.

## Example structure

These are the files used in the example, followed by a brief description of each file:

```text
parameter-api-python
├── app
│   ├── apis
│   │   └── keyvaluestore.proto
│   └── src
│       └── parameter.py
├── docker-compose.yml
├── Dockerfile
└── README.md
```

* **app/apis/keyvaluestore.proto** - Protobuf file to define Parameter-API messages and services
* **app/src/parameter.py** - App to retrieve Axis device parameters
* **docker-compose.yml** - Specifies the images used to run the application, and their interdependencies
* **Dockerfile** - Specifies how the application is built
* **README.md** - Instructions on how to build and run the application

## Requirements

Meet the following requirements to ensure compatibility with the example:

* Axis device
  * Chip: ARTPEC-{7-8} DLPU devices (e.g., Q1615 MkIII)
  * Firmware: 11.10 or higher
  * [Docker ACAP](https://github.com/AxisCommunications/docker-acap#installing) version 3.0 installed and started, using TLS with TCP and IPC socket and SD card as storage
  * [ACAP Runtime](https://github.com/AxisCommunications/acap-runtime#native-acap-application) installed and started
* Computer
  * Either [Docker Desktop](https://docs.docker.com/desktop/) version 4.11.1 or higher,
  * or [Docker Engine](https://docs.docker.com/engine/) version 20.10.17 or higher with BuildKit enabled using Docker Compose version 1.29.2 or higher

## Proxy settings

Depending on the network, you might need proxy settings in the following file: `~/.docker/config.json`.

For reference please see: https://docs.docker.com/network/proxy/.

## How to run the code

### Export the environment variable for the architecture

Export the `ARCH` variable depending on the architecture of your camera:

```sh
# For arm32
export ARCH=armv7hf

# For arm64
export ARCH=aarch64
```

### Build the Docker image

```sh
# Define app name
export APP_NAME=parameter-api

# Build
docker build --tag $APP_NAME --build-arg ARCH .
```

### Set your device IP address and clear Docker memory

```sh
DEVICE_IP=<actual camera IP address>
DOCKER_PORT=2376

docker --tlsverify --host tcp://$DEVICE_IP:$DOCKER_PORT system prune --all --force
```

If you encounter any TLS related issues, please see the TLS setup chapter regarding the `DOCKER_CERT_PATH` environment variable in the [Docker ACAP repository](https://github.com/AxisCommunications/docker-acap#securing-the-docker-acap-using-tls).

### Install the image and required server certificates

To use SSL/TLS server authentication and encryption, a server certificate is provided as a parameter to the application in the `docker-compose.yml` file:

```yaml
    entrypoint: python3 parameter.py /certificates/server.pem
```

Certificate files for TLS are created in the build process of this example and must be copied to ACAP Runtime folder on the device:

```sh
docker cp $(docker create $APP_NAME):/certificates .
scp certificates/* root@$DEVICE_IP:/usr/local/packages/acapruntime
```

Use SSH to change the ownership of the files on the device:

```sh
ssh root@$DEVICE_IP 'chown sdk /usr/local/packages/acapruntime/server.*'
```

After copying the server certificates onto the device, we have to make sure to enable TLS and then restart the ACAP Runtime.

```sh
AXIS_TARGET_PASSWORD='<password>'

# Enable TLS
curl --anyauth -u "root:$AXIS_TARGET_PASSWORD" "$DEVICE_IP/axis-cgi/param.cgi?action=update&acapruntime.UseTLS=yes"

# Restart ACAP
curl --anyauth -u "root:$AXIS_TARGET_PASSWORD" "$DEVICE_IP/axis-cgi/applications/control.cgi?package=acapruntime&action=restart"
```

where `<password>` is the password to the `root` user.

Finally install the Docker image to the device:

```sh
docker save $APP_NAME | docker --tlsverify --host tcp://$DEVICE_IP:$DOCKER_PORT load
```

### Run the container

With the application image on the device, it can be started using `docker-compose.yml`:

```sh
docker --tlsverify --host tcp://$DEVICE_IP:$DOCKER_PORT compose up

# Cleanup
docker --tlsverify --host tcp://$DEVICE_IP:$DOCKER_PORT compose down --volumes
```

where `ca.pem`, `cert.pem` and `key.pem` are the certificates generated when configuring TLS on Docker ACAP.

### The expected output

```text
parameter-api-python-parameter-api-1  | root.Brand.Brand : AXIS
parameter-api-python-parameter-api-1  | root.Brand.WebURL : http://www.axis.com
parameter-api-python-parameter-api-1  | root.Image.I0.Enabled : yes
parameter-api-python-parameter-api-1  | root.Brand.ProdFullName : AXIS Q1615 Mk III Network Camera
parameter-api-python-parameter-api-1  | root.Brand.ProdNbr : Q1615 Mk III
parameter-api-python-parameter-api-1  | root.invalid :
```

## License

**[Apache License 2.0](../LICENSE)**
