
# Mission
Our mission is to provide an excellent development expereince by enabling developers
to build new AI/ML applications for a smarter and safer world.

# Axis video analytics example applications
Video analytics ensures that video surveillance systems become smarter,
more accurate, more cost-effective and easier to manage. The most scalable
and flexible video analytics architecture is based on ‘intelligence at the
edge’, that is, processing as much of the video as possible in the network
cameras or video encoders themselves.

This not only uses the least amount of bandwidth but also significantly reduces
the cost and complexity of the network. Open application development platforms
such as Axis Camera Application Platform (ACAP) Facilitate the integration of
compatible third-party solutions, resulting in a quickly growing variety of
applications–general as well as specialized for different industries. The
growing number of video analytics applications creates new end-user benefits
and opens up new business possibilities.

# Getting started
This repository contains a set of application examples which aims to enrich the
developers analytics experience. All examples are using Docker framework and has a
README file in its directory which shows overview, example directory structure and
step-by-step instructions on how to run applications on the camera.

## Requirements
To get started following system requirements shall be met:
* Camera: Q1615-MkIII
* Docker version 19.03.5 or higher
* Debian Stretch or Ubuntu 18.04
* Firmware: Q1615-MkIII_9.80.1_24_fimage.bin
* Docker Daemon installed on the camera
* Dockerhub ID to pull images

## Supported architectures
The examples support the following architectures.
* armv7hf

## Example applications for video analytics
Below is the list of examples available in the respository.

* [Inference client](./inference-client/)
  * The example code is written in Python and implements the object detection on a
    video stream and still image from the camera.
* [Object-detector-cpp](./object-detector-cpp/)
  * The example code is written in C++ for object detection on the camera.
* [OpenCV-capture-app](./opencv-capture-app/)
  * The example code is written in C++ to capture camera properties such as time stamps,
   zoom, focus etc.

### DockerHub Images
There are two types of Docker images here: the ToolChain (SDK), and the API 3.1. These images can be used as the basis for custom built images for running your applications. The images needed are specified in the docker-compose files. All images are public and free to use for anyone.
* [Toolchain](https://hub.docker.com/repository/docker/axisecp/acap-toolchain) -  An Ubuntu-based Toolchain bundle with all tools for building and packaging an ACAP 3.1 application included.
* [API 3.1](https://hub.docker.com/repository/docker/axisecp/acap-api) - An Ubuntu-based API bundle with all API components (header and library files) included.

### Pre-trained models
* [COCO SSD MobileNet v1 model](https://www.tensorflow.org/lite/models/object_detection/overview#starter_model)


# How to work with Github repository
You can help to make this repo a better one using the following commands.

1. Fork it (git checkout ..)
2. Create your feature branch: git checkout -b <contr/my-new-feature>
3. Commit your changes: git commit -a
4. Push to the branch: git push origin <contr/my-new-feature>
5. Submit a pull request


# License
[Apache 2.0](LICENSE)

# Ref:
https://github.com/AxisCommunications/acap-application-examples-test
