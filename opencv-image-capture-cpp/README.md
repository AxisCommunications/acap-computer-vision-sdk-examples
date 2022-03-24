*Copyright (C) 2021, Axis Communications AB, Lund, Sweden. All Rights Reserved.*

# An OpenCV based example application on an edge device
This readme file explains how to build an OpenCV application.
It is achieved by using the ACAP Computer Vision SDK image, which contains
all the essential parts needed.

The ACAP Computer Vision SDK OpenCV comes with following modules:
core, imgproc, imgcodecs, videoio,objdetect, python3, video, as well as with Axis VDO integration
to enable capturing images directly from the camera. If you wish to use another set of modules,
the Dockerfile used to build the CV SDK's OpenCV module can be found in the [acap-computer-vision-sdk repository](https://github.com/AxisCommunications/acap-computer-vision-sdk/tree/master).

Together with this file you should be able to find a directory called
app, which contains the "capture.cpp" application which can
easily be compiled and run with the help of the tools and step by step
below.

## Requirements
To ensure compatibility with the examples, the following requirements shall be met:
* Camera: ARTPEC-{7-8} DLPU devices (e.g., Q1615 MkIII)
* docker-compose version 1.29 or higher
* Docker version 20.10.8 or higher
* Firmware: 10.9
* docker-acap installed on the camera
* docker-acap set to use TLS and external memory card

## Getting started
These instructions below will guide you on how to execute the code. Below is the structure and scripts used in the example:

```bash
opencv-image-capture-cpp
├── app
│   ├── Makefile
│   └── src
│       └── capture.cpp
├── docker-compose.yml
├── Dockerfile
└── README.md
```
* **capture.cpp**        - Example application to capture camera properties such as time stamps, zoom, focus etc.
* **Dockerfile**         - Docker file with the toolchain included to run the example.
* **docker-compose.yml** - Docker compose file contains the latest image of the example from dockerhub.
* **README.md**          - Step by step instructions on how to run the example.

### Limitations
* OpenCV module choice cannot be made in this version.
* In order to change the binary name it has to be done in the Makefile

### How to run the code
Begin by setting up some environment variables.
Below, we define the camera's IP, the desired app name and the path and version of the ACAP Computer Vision SDK.
```sh
# Set your camera IP address and clear docker memory
export AXIS_TARGET_IP=<actual camera IP address>
export DOCKER_PORT=2376
docker --tlsverify -H tcp://$AXIS_TARGET_IP:$DOCKER_PORT system prune -af
```
### Export environment variables for arm32 cameras
```sh
# Set environment variables
# ARCH defines what architecture to use (e.g., armv7hf, aarch64)
# RUNTIME_IMAGE defines what base image should be used for the application image 
export ARCH=armv7hf
export RUNTIME_IMAGE=arm32v7/ubuntu:20.04
export APP_NAME=acap-opencv-image-capture-cpp
```
### Export environment variables for arm64 cameras
```sh
export ARCH=aarch64
export RUNTIME_IMAGE=arm64v8/ubuntu:20.04
export APP_NAME=acap-opencv-image-capture-cpp
```

# Build and upload the application
```sh
docker build . -t $APP_NAME --build-arg ARCH --build-arg RUNTIME_IMAGE 


docker save $APP_NAME | docker --tlsverify -H tcp://$AXIS_TARGET_IP:$DOCKER_PORT  load
```

#### Run the container
```sh
docker-compose --tlsverify -H tcp://$AXIS_TARGET_IP:$DOCKER_PORT -f docker-compose.yml up

# Cleanup
docker-compose --tlsverify -H tcp://$AXIS_TARGET_IP:$DOCKER_PORT -f docker-compose.yml down -v
```

#### The expected output:
```bash
Setting Rotation 270: Done
Setting Rotation 180: Done
Setting Rotation  90: Done
Setting Rotation   0: Done
Configured image parameters in 72667µs
FRAME[ 0] 640x360 fd[7]  offs[0xc1000] size[0x56400]
FRAME[ 1] 640x360 fd[11] offs[0xc3000] size[0x56400]
FRAME[ 2] 640x360 fd[7]  offs[0xc5000] size[0x56400]
FRAME[ 3] 640x360 fd[11] offs[0xc1000] size[0x56400]
FRAME[ 4] 640x360 fd[7]  offs[0xc3000] size[0x56400]
FRAME[ 5] 640x360 fd[11] offs[0xc5000] size[0x56400]
FRAME[ 6] 640x360 fd[7]  offs[0xc1000] size[0x56400]
FRAME[ 7] 640x360 fd[11] offs[0xc3000] size[0x56400]
FRAME[ 8] 640x360 fd[7]  offs[0xc5000] size[0x56400]
FRAME[ 9] 640x360 fd[11] offs[0xc1000] size[0x56400]
FRAME[10] 640x360 fd[7]  offs[0xc3000] size[0x56400]
FRAME[11] 640x360 fd[11] offs[0xc5000] size[0x56400]
FRAME[12] 640x360 fd[7]  offs[0xc1000] size[0x56400]
FRAME[13] 640x360 fd[11] offs[0xc3000] size[0x56400]
FRAME[14] 640x360 fd[7]  offs[0xc5000] size[0x56400]
FRAME[15] 640x360 fd[11] offs[0xc1000] size[0x56400]
FRAME[16] 640x360 fd[7]  offs[0xc3000] size[0x56400]
FRAME[17] 640x360 fd[11] offs[0xc5000] size[0x56400]
FRAME[18] 640x360 fd[7]  offs[0xc1000] size[0x56400]
FRAME[19] 640x360 fd[11] offs[0xc3000] size[0x56400]
FRAME[20] 640x360 fd[7]  offs[0xc5000] size[0x56400]
FRAME[21] 640x360 fd[11] offs[0xc1000] size[0x56400]
FRAME[22] 640x360 fd[7]  offs[0xc3000] size[0x56400]
FRAME[23] 640x360 fd[11] offs[0xc5000] size[0x56400]
FRAME[24] 640x360 fd[7]  offs[0xc1000] size[0x56400]
FRAME[25] 640x360 fd[11] offs[0xc3000] size[0x56400]
FRAME[26] 640x360 fd[7]  offs[0xc5000] size[0x56400]
FRAME[27] 640x360 fd[11] offs[0xc1000] size[0x56400]
FRAME[28] 640x360 fd[7]  offs[0xc3000] size[0x56400]
FRAME[29] 640x360 fd[11] offs[0xc5000] size[0x56400]
Captured 30 frames in 973831µs
Optics: iCS
iteration[0] gain: 191.000000   expo: 503.000000   zoom: 1.862427   focus: 0.902723   f-number: 1.718091
iteration[1] gain: 191.000000   expo: 503.000000   zoom: 1.862427   focus: 0.902723   f-number: 1.718091
iteration[2] gain: 191.000000   expo: 503.000000   zoom: 1.862427   focus: 0.902723   f-number: 1.718091
Queried settings 3 times in 15344µs
```

#### OpenCV-VideoIO UniMatrix extensions

These extensions facilitate inter-process zero-copy by allowing buffers
to be forwarded across unix domain sockets.

Mandatory zero-copy extensions:
* CAP_PROP_UNIMATRIX_FD
  Buffer file descriptor
* CAP_PROP_UNIMATRIX_FD_OFFSET
  Buffer offset
* CAP_PROP_UNIMATRIX_FD_CAPACITY
  Buffer capacity
* CAP_PROP_UNIMATRIX_MAX_BUFFERS
  Maximum buffers in-flight

UniMatrix conforming backends must support at least *one* of these fourcc formats:
* NV12
* NV21
* RGB3

The most efficient color format should always be default.

UniMatrix conforming backends must support monochrome fourcc:
* Y800

Optional properties:
* CAP_PROP_UNIMATRIX_FNUMBER
  f-number

#### OpenCV-VideoIO Axis implementation

Axis supports all UniMatrix extensions:
* CAP_PROP_UNIMATRIX_FD
* CAP_PROP_UNIMATRIX_FD_OFFSET
* CAP_PROP_UNIMATRIX_FD_CAPACITY
* CAP_PROP_UNIMATRIX_MAX_BUFFERS
* CAP_PROP_UNIMATRIX_FNUMBER

These UniMatrix stream formats are not supported:
* NV21

Below stream formats are fully hardware-accelerated:
* Y800
* NV12
  This is the default native format of the camera.

Below stream formats are converted from the native format (i.e. it has a performance penalty):
* RGB3

These *stream* properties can be changed before capturing the first frame,
not when the stream is running:
* CAP_PROP_FPS
* CAP_PROP_FOURCC
* CAP_PROP_CHANNEL
* CAP_PROP_FRAME_WIDTH
* CAP_PROP_FRAME_HEIGHT
* CAP_PROP_UNIMATRIX_ROTATION
  Possible rotations include [0,90,180,270].
  Not every camera is required to support every rotation.

These *stream* properties are read-only:
* CAP_PROP_POS_MSEC
* CAP_PROP_POS_FRAMES

These *image* properties are read-only:
* CAP_PROP_ZOOM
  Zoom factor (1.0-)
* CAP_PROP_FOCUS
  Focus dioptre
* CAP_PROP_GAIN
  Gain in milli-dB
* CAP_PROP_EXPOSURE
  Exposure in µs
* CAP_PROP_UNIMATRIX_FNUMBER
  f-number
* CAP_PROP_UNIMATRIX_OPTICS_TYPE
  * CAP_UNIMATRIX_OPTICS_TYPE_MANUAL
    Manual zoom/focus/iris
  * CAP_UNIMATRIX_OPTICS_TYPE_DC
    Manual zoom/focus with DirectControl-iris
  * CAP_UNIMATRIX_OPTICS_TYPE_P
    Manual zoom/focus with Precise-iris
  * CAP_UNIMATRIX_OPTICS_TYPE_iCS
    Intelligent CS-mount
  * CAP_UNIMATRIX_OPTICS_TYPE_CAMBLOCK
    Camblock

These *image* properties are write-only:
* CAP_PROP_UNIMATRIX_EXPOSURE_MODE
  * CAP_UNIMATRIX_EXPOSURE_MODE_AUTO
    Automatic exposure
  * CAP_UNIMATRIX_EXPOSURE_MODE_HOLD
    Hold current exposure
  * CAP_PROP_UNIMATRIX_MAX_EXPOSURE_us
    Limit max automatic exposure time (unit: µs)

These *image* properties are read-write:
* CAP_PROP_UNIMATRIX_TONEMAPPING
  ToneMapping [0-100]
* CAP_PROP_UNIMATRIX_TEMPORAL_FILTER
  Temporal Noise-Filter [0-100]

## Proxy settings
Depending on the network, you might need proxy settings in the following file: `~/.docker/config.json`.

For reference please see: https://docs.docker.com/network/proxy/.

## License
**[Apache License 2.0](../LICENSE)**

## References
* https://docs.opencv.org/
