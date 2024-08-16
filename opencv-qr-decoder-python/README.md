*Copyright (C) 2021, Axis Communications AB, Lund, Sweden. All Rights Reserved.*

## OpenCV QR Decoder

This example shows a minimal QR code detector and decoder application written in Python.
It composes two different container images into an application that performs an inference using a deep learning model.

The first container contains the actual program built in this example. It then uses [gRPC](https://grpc.io/)/[protobuf](https://developers.google.com/protocol-buffers) to call the second container, that is used to capture images from the camera.

NumPy is used to preprocess the images and OpenCV to detect and decode any QR codes within the image.
Additionally, Python, NumPy and OpenBLAS are also copied from the Computer Vision SDK.

## Application layout

```text
opencv-qr-decoder-python
├── app
│   └── qr.py
├── docker-compose.yml
├── Dockerfile
└── README.md
```

* **qr.py**              - The application's main script
* **Dockerfile**         - Dockerfile specifying how the application runtime is built
* **docker-compose.yml** - docker-compose file specifying how/with what settings the application is started

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
export APP_NAME=acap-opencv-qr-decoder-python

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

Next, the built images needs to be uploaded to the device. This can be done through a registry or directly. In this case, the direct transfer is used by piping the compressed application directly to the device's Docker client:

```sh
docker save $APP_NAME | docker --tlsverify --host tcp://$DEVICE_IP:$DOCKER_PORT load
```

### Run the container

With the application image on the device, it can be started. As the example uses OpenCV, the OpenCV requirements will be included in `docker-compose.yml`, which is used to run the application:

```sh
docker --tlsverify --host tcp://$DEVICE_IP:$DOCKER_PORT compose up

# Terminate with Ctrl-C and cleanup
docker --tlsverify --host tcp://$DEVICE_IP:$DOCKER_PORT compose down --volumes
```

## License

**[Apache License 2.0](../LICENSE)**

## References

* https://docs.opencv.org/
