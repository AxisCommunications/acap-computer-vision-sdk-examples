*Copyright (C) 2021, Axis Communications AB, Lund, Sweden. All Rights Reserved.*

## OpenCV QR Decoder
This example shows a minimal QR code detector and decoder application written in Python.
It involves using OpenCV to create a video stream, NumPy to preprocess
the images and OpenCV to detect and decode any QR codes within the image. The example uses the
[ACAP Computer Vision SDK](https://github.com/AxisCommunications/acap-computer-vision-sdk) to get
OpenCV with VDO integration, allowing capture of a video stream through OpenCV's standard
VideoCapture class. Additionally, Python, NumPy and OpenBLAS are also copied
from the Computer Vision SDK.

## Requirements
To ensure compatibility with the examples, the following requirements shall be met:
* Camera: ARTPEC-7 DLPU devices (e.g., Q1615 MkIII)
* docker-compose version 1.29 or higher
* Docker version 20.10.8 or higher
* Firmware: 10.7
* docker-acap installed on the camera
* docker-acap set to use TLS and external memory card

## Application layout
```bash
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

### Running the application
Begin by setting up some environment variables.
Below, we define the camera's IP, the desired app name and the path and version of the ACAP Computer Vision SDK.
```sh
# CV SDK configuration# Set your camera IP address and clear docker memory
export AXIS_TARGET_IP=<actual camera IP address>
docker --tlsverify -H tcp://$AXIS_TARGET_IP:2376 system prune -af

# Set environment variables
export ARCH=armv7hf
export REPO=axisecp
export RUNTIME_IMAGE=arm32v7/ubuntu:20.04
export APP_NAME=acap-opencv-qr-decoder-python

# Build and load the application on the camera
docker build . -t $APP_NAME --build-arg ARCH --build-arg REPO --build-arg RUNTIME_IMAGE
docker save $APP_NAME | docker --tlsverify -H tcp://$AXIS_TARGET_IP:2376 load

# Run the application on the camera
docker-compose --tlsverify -H tcp://$AXIS_TARGET_IP:2376 up

# Terminate with ctrl-C and cleanup
docker-compose --tlsverify -H tcp://$AXIS_TARGET_IP:2376 down -v
```

## License
**[Apache License 2.0](../LICENSE)**


## References
* https://docs.opencv.org/
