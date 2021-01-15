# An inference example application on an edge device
This example is written in Python and implements the following object detection scenarios:
 - Run a video streaming inference on camera
 - Run a still image inference on camera

## Overview
This example composes three different container images into an application that performs object detection using a deep learning model.

The first container contains the actual program being built here, which uses OpenCV to capture pictures from the camera and modifies them to fit the input required by the model. It then uses grpc/protobuf to call the second container, the "inference-server", that performs the actual inference. The inference server implements the TensorFlow Serving API.

Lastly, there is a third container that holds the deep learning model, which is put into a volume that is accessible to the other two images.

**larod-inference server**\
Uses larod service in the camera firmware with model in docker image
axisecp/acap-dl-models:ssdlite-mobilenet-v2

```bash
# Extract model from docker image
docker cp $(docker create axisecp/acap-dl-models:ssdlite-mobilenet-v2):/ .

model
├── ssdlite-mobilenet-v2 - model for CPU
├── ssdlite-mobilenet-v2-tpu - model for TPU
└── objects.txt - list of object labels
```

## Example Structure
Following are the list of files and a brief description of each file in the example
```bash
inference-client
├── camera-image.yml
├── camera-video.yml
├── detector.py
├── Dockerfile
├── dog416.png
└── README.md
```

* **Dockerfile** - Build Docker image with inference client for camera
* **detector.py** - The inference client main program
* **camera-image.yml** - Docker compose file for still image example using larod inference service
* **camera-video.yml** - Docker compose file for streaming video example using larod inference service
* **dog416.png** - Example image

## Prerequisites
To get started following system requirements shall be met:
* Docker version 19.03.5 or higher
* Debian Stretch or Ubuntu 18.04
* Firmware: Q1615-MkIII_10.2.0_fimage.bin
* Docker Daemon installed on the camera
* Dockerhub ID to pull images

## How to run the code
The video tutorial shows the steps below how to build and run the code.
<div align="center">
      <a href="https://www.youtube.com/embed/h7yjFf0jGhs">
         <img src="https://img.youtube.com/vi/h7yjFf0jGhs/0.jpg">
      </a>
</div>
Build a Docker inference-client image, example:

```sh
# Find build folder
cd acap-application-examples/inference-client

# Adjust some environment variables to your preference, then build and push to docker repo
export REPO=axisecp
export ARCH=armv7hf
export UBUNTU_VERSION=20.04
export INFERENCE_CLIENT=$REPO/inference-client:1.1.0-$ARCH-ubuntu$UBUNTU_VERSION
export PYTHON_TFSERVING=$REPO/python-tfserving:1.1.0-$ARCH-ubuntu$UBUNTU_VERSION
docker build --build-arg PYTHON_TFSERVING -t $INFERENCE_CLIENT .
docker push $INFERENCE_CLIENT
```

There are two options available in this example:

```sh
# Set your camera IP address
export AXIS_TARGET_IP=<actual camera IP address>

# Clear docker memory on camera
docker -H tcp://$AXIS_TARGET_IP system prune -af

# **OPT 1** - Use the following command to run the video streaming inference on the camera, example:
docker-compose -H tcp://$AXIS_TARGET_IP:2375 -f camera-video.yml up

# **OPT 2** - Use the following command to run still image inference on the camera, example:
docker-compose -H tcp://$AXIS_TARGET_IP:2375 -f camera-image.yml up
```

### The expected output:
`docker-compose -H tcp://$AXIS_TARGET_IP:2375 -f camera-video.yml up`
```
....
object-detector_1           | 1 Objects found
object-detector_1           | person
```

`docker-compose -H tcp://$AXIS_TARGET_IP:2375 -f camera-image.yml up`
```
....
inference-client_1          | 3 Objects found
inference-client_1          | bicycle
inference-client_1          | dog
inference-client_1          | car
```
## License:
Apache 2.0