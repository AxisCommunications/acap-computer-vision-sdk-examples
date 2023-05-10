*Copyright (C) 2021, Axis Communications AB, Lund, Sweden. All Rights Reserved.*

# A minimal Python application

This example demonstrates how to create a simple Python application using the ACAP Computer Vision SDK and run it on an edge device.

Going from zero to a Python application running on an AXIS device is quite easy. First, the application script is written, as in the hello-world script in [app/simply_hello.py](app/simply_hello.py). Next, the [Dockerfile](Dockerfile) which build the application image is constructed. This needs to pull in packages from the ACAP Computer Vision SDK, as is done using the `COPY` commands. Finally, the application needs to be built and uploaded, as is specified below.

## Example structure

Following are the list of files and a brief description of each file in the example

```text
hello-world-python
├── app
│   └── simply_hello.py
├── Dockerfile
├── docker-compose.yml
└── README.md
```

* **simply_hello.py** - A Python script that prints "Hello World"
* **Dockerfile** - Build instructions for the application image that is run on the camera
* **docker-compose.yml** - A docker-compose file that specifies how the application is run

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

# For arm64
export ARCH=aarch64
```

### Build the Docker image

With the architecture defined, the `hello-world-python` image can be built. The environment variables are supplied as build arguments such that they are made available to docker during the build process:

```sh
# Define app name
export APP_NAME=hello-world-python

docker build --tag $APP_NAME --build-arg ARCH .
```

### Set your device IP address and clear Docker memory

```sh
DEVICE_IP=<actual camera IP address>
DOCKER_PORT=2376

docker --tlsverify --host tcp://$DEVICE_IP:$DOCKER_PORT system prune --all --force
```

If you encounter any TLS related issues, please see the TLS setup chapter regarding the `DOCKER_CERT_PATH` environment variable in the [Docker ACAP repository](https://github.com/AxisCommunications/docker-acap).

### Install the image

Next, the built image needs to be uploaded to the device. This can be done through a registry or directly. In this case, the direct transfer is used by piping the compressed application directly to the device's docker client:

```sh
docker save $APP_NAME | docker --tlsverify --host tcp://$DEVICE_IP:$DOCKER_PORT load
```

### Run the container

With the application image on the device, it can be started using `docker-compose.yml`:

```sh
docker-compose --tlsverify --host tcp://$DEVICE_IP:$DOCKER_PORT up

# Cleanup
docker-compose --tlsverify --host tcp://$DEVICE_IP:$DOCKER_PORT down
```

The expected output from the application is simply:

```text
Hello World!
```

## License

**[Apache License 2.0](../LICENSE)**
