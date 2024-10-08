*Copyright (C) 2022, Axis Communications AB, Lund, Sweden. All Rights Reserved.*

# ACAP Computer Vision SDK examples

[![CI](https://github.com/AxisCommunications/acap-computer-vision-sdk-examples/actions/workflows/ci.yml/badge.svg)](https://github.com/AxisCommunications/acap-computer-vision-sdk-examples/actions/workflows/ci.yml)

> [!IMPORTANT]
> The ACAP Computer Vision solution is deprecated from AXIS OS 12.0.
>
> - The deprecation of the ACAP Computer Vision solution only affects container applications.
>   It will still be possible to build and run analytics, computer vision and machine learning applications on Axis devices using
>   ACAP Native SDK. Examples can be found in the
>   [ACAP Native SDK examples](https://github.com/AxisCommunications/acap-native-sdk-examples) repository.
>   For more context about this transition, please visit the [Enhanced ACAP SDK news article](https://www.axis.com/developer-community/news/enhanced-acap-sdk).
> - `armv7hf` is no longer supported after AXIS OS 11.11.
> - `aarch64` will be supported until the end of the AXIS OS 12 LTS cycle.
>   After this period, the components of the Computer Vision solution will no longer
>   receive updates or support.
> - From AXIS OS 12.0, the artifact ACAP Computer Vision SDK in form of a
>   container image is removed and utility libraries and scripts have been
>   moved to [ACAP Runtime](https://github.com/AxisCommunications/acap-runtime).
>   This means that the ACAP Computer Vision SDK is no longer needed to run container applications.

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
* [ACAP runtime](https://github.com/AxisCommunications/acap-runtime) is used in a few examples to get access to [APIs](https://axiscommunications.github.io/acap-documentation/docs/api/computer-vision-sdk-apis.html). ACAP Runtime is a container image and the example applications will pull it from Docker Hub when starting.

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
