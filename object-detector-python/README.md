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
`axisecp/acap-dl-models:ssdlite-mobilenet-v2`

The docker image containing the model has a layout as shown below. What model to use is specified by path in the docker-compose file.
```bash
model
├── ssdlite-mobilenet-v2 - model for CPU
├── ssdlite-mobilenet-v2-tpu - model for TPU
└── objects.txt - list of object labels
```

## Example Structure
Following are the list of files and a brief description of each file in the example
```bash
object-detector-python
├── app
├── |- detector.py
├── |- dog416.png
├── docker-compose.yml
├── static-image.yml
├── Dockerfile
├── Dockerfile.model
└── README.md
```

* **detector.py** - The inference client main program
* **dog416.png** - Static image used with static-image.yml
* **docker-compose.yml** - Docker compose file for streaming camera video example using larod inference service
* **static-image.yml** - Docker compose file for static image debug example using larod inference service
* **Dockerfile** - Build Docker image with inference client for camera
* **Dockerfile.model** - Build Docker image with inference model

## Prerequisites
To get started following system requirements shall be met:
* Camera: Q1615-MkIII
* docker-compose version 1.27.4 or higher
* Docker version 19.03.5 or higher
* Firmware: 10.6
* ACAP4 installed on the camera

## How to run the code
Build a Docker object-detector-python image, example:

```sh
# Adjust some environment variables to your preference, then build and push to docker repo
export REPO=axisecp
export ARCH=armv7hf
export RUNTIME_IMAGE=arm32v7/ubuntu:20.04

# To allow retrieval of the image from the cloud
# this should be a repository that you can push to
# and that your camera can pull from, i.e., substitute
# axisecp for your own repository
export APP_NAME=axisecp/acap-object-detector-python
docker build . -t $APP_NAME --build-arg REPO --build-arg ARCH --build-arg RUNTIME_IMAGE
docker push $APP_NAME
```

* Build docker container with inference models:
```sh
# To allow retrieval of the image from the cloud
# this should be a repository that you can push to
# and that your camera can pull from, i.e., substitute
# axisecp for your own repository
export MODEL_NAME=axisecp/acap-dl-models:1.0
docker build . -f Dockerfile.model -t $MODEL_NAME
docker push $MODEL_NAME
```

There are two options available in this example:

```sh
# Set your camera IP address
export AXIS_TARGET_IP=<actual camera IP address>

# Clear docker memory on camera
docker -H tcp://$AXIS_TARGET_IP system prune -af

# **OPT 1** - Use the following command to run the video streaming inference on the camera, example:
docker-compose -H tcp://$AXIS_TARGET_IP:2375 up

# **OPT 2** - Use the following command to run static image inference on the camera, example:
docker-compose -H tcp://$AXIS_TARGET_IP:2375 -f static-image.yml up
```

### The expected output:
`docker-compose -H tcp://$AXIS_TARGET_IP:2375 up`
```
....
object-detector_1           | 1 Objects found
object-detector_1           | person
```

`docker-compose -H tcp://$AXIS_TARGET_IP:2375 -f static-image.yml up`
```
....
object-detector-python_1          | 3 Objects found
object-detector-python_1          | bicycle
object-detector-python_1          | dog
object-detector-python_1          | car
```
## License:
Apache 2.0
