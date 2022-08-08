# ACAP Computer Vision SDK examples

[![CI](https://github.com/AxisCommunications/acap-computer-vision-sdk-examples/actions/workflows/ci.yml/badge.svg)](https://github.com/AxisCommunications/acap-computer-vision-sdk-examples/actions/workflows/ci.yml)

## Mission

Our mission is to provide an excellent development experience by enabling developers
to build new AI/ML applications for a smarter and safer world.

## Axis video analytics example applications

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

## Getting started

This repository contains a set of application examples which aims to enrich the
developers analytics experience. All examples are using Docker framework and has a
README file in its directory which shows overview, example directory structure and
step-by-step instructions on how to run applications on the camera.

### Requirements

To ensure compatibility with the examples, the following requirements shall be met:

* Camera: ARTPEC-7 DLPU or ARTPEC8 devices (e.g., Q1615 MkIII, Q1656)
* docker-compose version 1.29 or higher
* Docker version 20.10.8 or higher
* Firmware: 10.7 or higher
  * [Docker ACAP](https://github.com/AxisCommunications/docker-acap) installed and started, using TLS and SD card as storage

### Supported architectures

The examples support the following architectures:

* armv7hf
* aarch64

### Example applications for video analytics

Below is a list of examples available in the repository:

* [hello-world](./hello-world/)
  * A Python example which builds a simple hello-world application.
* [hello-world-crosscompiled](./hello-world-crosscompiled/)
  * A C++ example which cross compiles a simple OpenCV hello-world application.
* [minimal-ml-inference](./minimal-ml-inference/)
  * A Python example to build a minimal machine learning inference application.
* [object-detector-cpp](./object-detector-cpp/)
  * A C++ example which runs object detection on the camera.
* [object-detector-python](./object-detector-python/)
  * A Python example which implements object detection on a
    video stream from the camera.
* [opencv-image-capture-cpp](./opencv-image-capture-cpp/)
  * A C++ example which captures camera frames and properties such as time stamps, zoom, focus etc., through OpenCV.
* [opencv-qr-decoder-python](./opencv-qr-decoder-python/)
  * A Python example which detects and decodes QR codes in the video stream using OpenCV.
* [parameter-api](./parameter-api/)
  * A C++ example which reads camera parameters using the beta version of the Parameter-API.
* [pose-estimator-with-flask](./pose-estimator-with-flask/)
  * A Python example which implements pose estimation on a video stream from the camera, and publish the output on a video stream using flask.
* [web-server](./web-server/)
  * A C++ example which runs a Monkey web server on the camera.

### Docker Hub images

The examples are based on the [ACAP Computer Vision SDK](https://github.com/AxisCommunications/acap-computer-vision-sdk).
This SDK is an image which contains APIs and tooling to build computer vision apps for running on camera, with support for C/C++ and Python.
Additionally, there is the [ACAP Native SDK](https://github.com/AxisCommunications/acap-native-sdk), which is more geared towards building ACAPs that uses AXIS-developed APIs directly, and primarily does so using C/C++.

## How to work with Github repository

You can help to make this repo a better one using the following commands.

1. Fork it (git checkout ..)
2. Create your feature branch: git checkout -b <contr/my-new-feature>
3. Commit your changes: git commit -a
4. Push to the branch: git push origin <contr/my-new-feature>
5. Submit a pull request

## License

[Apache 2.0](LICENSE)
