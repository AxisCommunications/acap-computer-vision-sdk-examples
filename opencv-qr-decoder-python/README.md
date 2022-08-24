*Copyright (C) 2021, Axis Communications AB, Lund, Sweden. All Rights Reserved.*

## OpenCV QR Decoder

This example shows a minimal QR code detector and decoder application written in Python.
It involves using OpenCV to create a video stream, NumPy to preprocess
the images and OpenCV to detect and decode any QR codes within the image. The example uses the
[ACAP Computer Vision SDK](https://github.com/AxisCommunications/acap-computer-vision-sdk) to get
OpenCV with VDO integration, allowing capture of a video stream through OpenCV's standard
VideoCapture class. Additionally, Python, NumPy and OpenBLAS are also copied
from the Computer Vision SDK.

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

To ensure compatibility with the examples, the following requirements shall be met:

* Camera: ARTPEC-{7-8} DLPU devices (e.g., Q1615 MkIII)
* docker-compose version 1.29 or higher
* Docker version 20.10.8 or higher
* Firmware: 10.9
* [Docker ACAP](https://github.com/AxisCommunications/docker-acap) installed and started, using TLS and SD card as storage

## How to run the code

### Export the environment variable for the architecture

Export the ARCH variable depending on the architecture of your camera

```sh
# For arm32
export ARCH=armv7hf
# For arm64
export ARCH=aarch64
```

### Set your camera IP address define APP name and clear Docker memory

```sh
# Set camera IP
export AXIS_TARGET_IP=<actual camera IP address>
export DOCKER_PORT=2376

# Define APP name
export APP_NAME=acap-opencv-qr-decoder-python

# Clean docker memory
docker --tlsverify -H tcp://$AXIS_TARGET_IP:$DOCKER_PORT system prune -af
```

### Build and run the images

With the environment setup, the `acap-opencv-qr-decoder-python` image can be built. The environment variables are supplied as build arguments such that they are made available to docker during the build process:

```sh
docker build . -t $APP_NAME --build-arg ARCH
```

Next, the built image needs to be uploaded to the device. This can be done through a registry or directly. In this case, the direct transfer is used by piping the compressed application directly to the device's docker client:

```sh
docker save $APP_NAME | docker --tlsverify -H tcp://$AXIS_TARGET_IP:$DOCKER_PORT  load
```

With the application image on the device, it can be started. As the example uses OpenCV, the OpenCV requirements will be included in `docker-compose.yml`, which is used to run the application:

```sh
docker-compose --tlsverify -H tcp://$AXIS_TARGET_IP:$DOCKER_PORT up

# Cleanup after execution
docker-compose --tlsverify -H tcp://$AXIS_TARGET_IP:$DOCKER_PORT down -v
```

## License

**[Apache License 2.0](../LICENSE)**

## References

* https://docs.opencv.org/
