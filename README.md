*Copyright (C) 2022, Axis Communications AB, Lund, Sweden. All Rights Reserved.*

# ACAP Computer Vision SDK examples

[![CI](https://github.com/AxisCommunications/acap-computer-vision-sdk-examples/actions/workflows/ci.yml/badge.svg)](https://github.com/AxisCommunications/acap-computer-vision-sdk-examples/actions/workflows/ci.yml)

> **Note:**
>
> - New Axis products released on AXIS OS 12.x will not have container support.
> - [All products with existing container support](https://www.axis.com/support/tools/product-selector/shared/%5B%7B%22index%22%3A%5B10%2C0%5D%2C%22value%22%3A%22ARTPEC-8%22%7D%2C%7B%22index%22%3A%5B10%2C2%5D%2C%22value%22%3A%22Yes%22%7D%5D)
>   will be supported until end of 2031 when [AXIS OS 2026 LTS](https://help.axis.com/en-us/axis-os) reaches end of life.
> - The recommended way to build analytics, computer vision and machine learning applications on Axis devices with ACAP support,
>   is to use the ACAP Native SDK. For usage see the [acap-native-sdk-examples](https://github.com/AxisCommunications/acap-native-sdk-examples)
>   repo.
> - The ACAP Computer Vision SDK has been archived as its components have been refactored:
>   utility libraries and scripts are now available in [ACAP Runtime](https://github.com/AxisCommunications/acap-runtime).
>   For usage of the new setup, see the [examples](https://github.com/AxisCommunications/acap-computer-vision-sdk-examples)
>   repository.

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

## Requirements

### Supported architectures

The examples support the following architectures:

* aarch64

### Required components

The example applications make use of two special components:

* [Docker ACAP](https://github.com/AxisCommunications/docker-acap) needs to be installed and running on the Axis device. It's required to deploy and run the example application.
* [ACAP runtime](https://github.com/AxisCommunications/acap-runtime) is used in a few examples to get access to [APIs](https://developer.axis.com/acap/api/computer-vision-sdk-apis). ACAP Runtime is a container image and the example applications will pull it from Docker Hub when starting.

## Example applications

Below is a list of examples available in the repository:

* [hello-world-python](./hello-world-python/)
  * A Python example which builds a simple hello-world application.
* [minimal-ml-inference](./minimal-ml-inference/)
  * A Python example to build a minimal machine learning inference application.
* [object-detector-python](./object-detector-python/)
  * A Python example which implements object detection on a
    video stream from the camera.
* [opencv-qr-decoder-python](./opencv-qr-decoder-python/)
  * A Python example which detects and decodes QR codes in the video stream using OpenCV.
* [parameter-api-python](./parameter-api-python/)
  * A Python example which reads camera parameters using the beta version of the Parameter-API.
* [pose-estimator-with-flask](./pose-estimator-with-flask/)
  * A Python example which implements pose estimation on a video stream from the camera, and publish the output on a video stream using flask.
* [web-server](./web-server/)
  * A C++ example which runs a Monkey web server on the camera.

## How to work with Github repository

You can help to make this repo a better one using the following commands.

1. Fork it (git checkout ...)
2. Create your feature branch: git checkout -b <contr/my-new-feature>
3. Commit your changes: git commit -a
4. Push to the branch: git push origin <contr/my-new-feature>
5. Submit a pull request

## License

[Apache 2.0](LICENSE)
