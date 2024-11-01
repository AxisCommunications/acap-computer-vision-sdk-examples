*Copyright (C) 2021, Axis Communications AB, Lund, Sweden. All Rights Reserved.*

# A minimal Python application

This example demonstrates how to create a simple containerized Python application and run it on an edge device.

Going from zero to a Python application running on an Axis device is quite easy. First, the application script is written, as in the hello-world script in [app/simply_hello.py](app/simply_hello.py). Next, the [Dockerfile](Dockerfile) which builds the application image is constructed. The application needs to be built and uploaded, as is specified below.

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
  * Chip: ARTPEC-8 DLPU devices (e.g., Q1656)
  * Firmware: 11.10 or higher
  * [Docker ACAP](https://github.com/AxisCommunications/docker-acap#installing) version 3.0 installed and started, using TLS with TCP and IPC socket and SD card as storage
* Computer
  * Either [Docker Desktop](https://docs.docker.com/desktop/) version 4.11.1 or higher,
  * or [Docker Engine](https://docs.docker.com/engine/) version 20.10.17 or higher with BuildKit enabled using Docker Compose version 1.29.2 or higher

## How to run the code

### Build the Docker image

Define and export the application image name in `APP_NAME` for use in the Docker Compose file.

```sh
export APP_NAME=hello-world-python

# Install qemu to allow build for a different architecture
docker run --rm --privileged multiarch/qemu-user-static --credential yes --persistent yes

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

Browse to the application page of the Axis device:

```sh
http://<AXIS_DEVICE_IP>/index.html#apps
```

Click on the tab `Apps` in the device GUI and enable `Allow unsigned apps` toggle.

Next, the built image needs to be uploaded to the device. This can be done through a registry or directly. In this case, the direct transfer is used by piping the compressed application directly to the device's Docker client:

```sh
docker save $APP_NAME | docker --tlsverify --host tcp://$DEVICE_IP:$DOCKER_PORT load
```

### Run the container

With the application image on the device, it can be started using `docker-compose.yml`:

```sh
docker --tlsverify --host tcp://$DEVICE_IP:$DOCKER_PORT compose up

# Cleanup
docker --tlsverify --host tcp://$DEVICE_IP:$DOCKER_PORT compose down
```

The expected output from the application is simply:

```text
Hello World!
```

## License

**[Apache License 2.0](../LICENSE)**
