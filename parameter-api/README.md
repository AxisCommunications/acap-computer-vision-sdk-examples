*Copyright (C) 2022, Axis Communications AB, Lund, Sweden. All Rights Reserved.*

# A Beta version of Parameter-API example application on an edge device

A network device from Axis has a lot of parameters and in this example we will use the Parameter-API, a service in [ACAP runtime](https://hub.docker.com/r/axisecp/acap-runtime) to read the values of a set of parameters and print them to the application log. The Parameter-API can read parameters, but cannot read parameter groups nor set parameter values.

The example is written in C++ and communicates with the [ACAP runtime](https://hub.docker.com/r/axisecp/acap-runtime) using gRPC on an Unix Domain Socket (UDS). ACAP runtime acts as a server that exposes the Parameter-API.

## Example structure
Below is the structure of the example with a brief description of scripts.
```sh
parameter-api
├── app
│   ├── apis
│   │   └── keyvaluestore.proto - Protobuf file to define parameter-api messages and services.
│   ├── src
│   │   └── parameter.cpp - App to retrieve Axis device parameters.
│   └── Makefile - Used by the make tool to build the program.
├── docker-compose.yml - Specifies the images used to run the application, and their interdependencies.
├── Dockerfile - Specifies how the application is built.
└── README.md - Instructions on how to build and run the application.
```

## Requirements
To ensure compatibility with the examples, the following requirements shall be met.

* Network device
  * Chip: ARTPEC-7 and ARTPEC-8 (e.g., Q1615 Mk III or Q1656)
  * Firmware: 10.9 or higher
  * [Docker ACAP](https://github.com/AxisCommunications/docker-acap) installed and started, using TLS and SD card as storage
  * [ACAP runtime](https://hub.docker.com/r/axisecp/acap-runtime) installed and started, using TLS
* Computer
  * Docker v20.10.8 or higher
  * docker-compose v1.29 or higher

## Proxy settings
Depending on the network, you might need proxy settings in the following file: `~/.docker/config.json`.

For reference please see: https://docs.docker.com/network/proxy/.

## How to run the code
### Build the parameter-api image
```sh
# Set your device IP address and clear docker memory
export AXIS_TARGET_IP=<actual device IP address>
docker --tlsverify -H tcp://$AXIS_TARGET_IP:2376 system prune -af

# Set environment variables for arm32 devices (ARTPEC-7)
export ARCH=armv7hf
export APP_NAME=parameter-api

# Set environment variables for arm64 devices (ARTPEC-8)
export ARCH=aarch64
export APP_NAME=parameter-api

# Build parameter-api
docker build . -t $APP_NAME --build-arg ARCH
```

### Install the image and required server certificates
This example uses an insecure gRPC communication channel. To use SSL/TLS server
authentication and encryption, a server certificate is provided as the first
parameter to the application in the docker-compose.yml file:

```yaml
    command: /usr/bin/parameter /models/server.pem
```

Certificate files for TLS are created in the build process of this example and
must be copied to the ACAP runtime folder on the device:

```sh
docker cp $(docker create $APP_NAME):/models .
scp models/* root@$AXIS_TARGET_IP:/usr/local/packages/acapruntime
```

Use SSH to change the ownership of the files on the device:
```sh
ssh root@$AXIS_TARGET_IP 'chown sdk /usr/local/packages/acapruntime/server.*'
```

Finally install the image to the device:
```sh
docker save $APP_NAME | docker --tlsverify -H tcp://$AXIS_TARGET_IP:2376 load
```

### Run the image
```sh
# Use the following command to run the example on the device
docker-compose --tlsverify -H tcp://$AXIS_TARGET_IP:2376 up

# Terminate with Ctrl-C and cleanup if needed
docker-compose --tlsverify -H tcp://$AXIS_TARGET_IP:2376 down -v
```

### The expected output:
```
parameter-api_1  | root.Brand.Brand : AXIS
parameter-api_1  | root.Brand.WebURL : http://www.axis.com
parameter-api_1  | root.Image.I0.Enabled : yes
parameter-api_1  | root.Brand.ProdFullName : AXIS Q1615 Mk III Network Camera
parameter-api_1  | root.Brand.ProdNbr : Q1615 Mk III
parameter-api_1  | root.invalid :
```

## License
**[Apache License 2.0](../LICENSE)**
