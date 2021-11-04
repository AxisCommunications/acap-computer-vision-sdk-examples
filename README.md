
# Mission
Our mission is to provide an excellent development experience by enabling developers
to build new AI/ML applications for a smarter and safer world.

# Axis video analytics example applications
Video analytics ensures that video surveillance systems become smarter,
more accurate, more cost-effective and easier to manage. The most scalable
and flexible video analytics architecture is based on ‘intelligence at the
edge’, that is, processing as much of the video as possible in the network
cameras or video encoders themselves.

This not only uses the least amount of bandwidth but also significantly reduces
the cost and complexity of the network. Open application development platforms
such as Axis Camera Application Platform (ACAP) facilitate the integration of
compatible third-party solutions, resulting in a quickly growing variety of
applications – general as well as specialized for different industries. The
growing number of video analytics applications creates new end-user benefits
and opens new business possibilities.

# Getting started
This repository contains a set of application examples which aims to enrich the
developers analytics experience. All examples are using Docker framework and has a
README file in its directory which shows overview, example directory structure and
step-by-step instructions on how to run applications on the camera.

## Requirements
To ensure compatibility with the examples, the following requirements shall be met:
* Camera: ARTPEC-7 DLPU devices (e.g., Q1615 MkIII)
* docker-compose version 1.29 or higher
* Docker version 20.10.8 or higher
* Firmware: 10.7
* docker-acap installed on the camera
* docker-acap set to TLS and external memory card

## Supported architectures
The examples support the following architectures.
* armv7hf

## Example applications for video analytics
Below is a list of examples available in the repository:

* [object-detector-python](./object-detector-python/)
  * A Python example which implements object detection on a
    video stream from the camera.
* [object-detector-cpp](./object-detector-cpp/)
  * A C++ example which runs object detection on the camera.
* [opencv-image-capture-cpp](./opencv-image-capture-cpp/)
  * A C++ example which captures camera frames and properties such as time stamps, zoom, focus etc., through OpenCV.
* [opencv-qr-decoder-python](./opencv-qr-decoder-python/)
  * A Python example which detects and decodes QR codes in the video stream using OpenCV.
* [web-server](./web-server/)
  * A C++ example which runs a Monkey web server on the camera.

## Dockerhub images
The examples are based on the [ACAP Computer Vision SDK](https://github.com/AxisCommunications/acap-computer-vision-sdk).
This SDK is an image which contains APIs and tooling to build computer vision apps for running on camera, with support for C/C++ and Python.
Additionally, there is the [ACAP Native SDK](https://github.com/AxisCommunications/acap-native-sdk), which is more geared towards building ACAPs that uses AXIS-developed APIs directly, and primarily does so using C/C++.

The examples also use the following images:
* [larod-inference-server](https://hub.docker.com/repository/docker/axisecp/larod-inference-server) - Inference server implemented on top of Larod API
* [acap-dl-models](https://hub.docker.com/repository/docker/axisecp/acap-dl-models) - A container containing deep learning models used by the different examples

# How to work with Github repository
You can help to make this repo a better one using the following commands.

1. Fork it (git checkout ..)
2. Create your feature branch: git checkout -b <contr/my-new-feature>
3. Commit your changes: git commit -a
4. Push to the branch: git push origin <contr/my-new-feature>
5. Submit a pull request


# License
[Apache 2.0](LICENSE)
