 *Copyright (C) 2020, Axis Communications AB, Lund, Sweden. All Rights Reserved.*

# A boundingbox based ACAP3 application on an edge device
This README file explains how to build an ACAP3 application using the boundingbox API. It is achieved by using the containerized Axis API and toolchain images.

Together with this README file, you should be able to find a directory called app. That directory contains the "boundingbox example" application source code which can easily be compiled and run with the help of the tools and step by step below.

The boundingbox library aims to be a easy-to-use high-performance library for drawing plain boxes. For more advanced use-cases it is recommended to use Axoverlay together with cairo or similar libraries.

This example illustrates how to draw boundingboxes on different video channels.

## Getting started
These instructions will guide you on how to execute the code. Below is the structure and scripts used in the example:

```bash
boundingbox
├── app
│   ├── LICENSE
│   ├── Makefile
│   └── bbox_example.cc
├── Dockerfile
└── README.md
```

* **Dockerfile** - Docker file with the specified Axis toolchain and API container to build the example specified.
* **app/LICENSE** - Text file which lists all open source licensed source code distributed with the application.
* **app/Makefile** - Makefile containing the build and link instructions for building the ACAP3 application.
* **app/bbox_example.cc** - Application that draws boundingboxes in C++.
* **README.md** - Step by step instructions on how to run the example.

### Limitations
* The example is done for the aarch64 architecture, but it is possible to use with armv7hf architecture.
* Supports Ambarella-S5L, Ambarella-CV25 and Artpec-7+.

### How to run the code
Below is the step by step instructions on how to execute the program. So basically starting with the generation of the .eap file to running it on a device:

#### Build the application
Standing in your working directory run the following commands:

> [!IMPORTANT]
> *Depending on the network you are connected to,
The file that needs those settings is: *~/.docker/config.json.*
For reference please see: https://docs.docker.com/network/proxy/ and a
[script for Axis device here](../FAQs.md#HowcanIset-upnetworkproxysettingsontheAxisdevice?).*

```bash
docker build --tag <APP_IMAGE> .
```

<APP_IMAGE> is the name to tag the image with, e.g., boundingboxexample:1.0

Copy the result from the container image to a local directory build:

```bash
docker cp $(docker create  <APP_IMAGE>):/opt/app ./build
```

The working dir now contains a build folder with the following files:

```bash
boundingbox
├── build
│   ├── LICENSE
│   ├── Makefile
│   ├── package.conf
│   ├── package.conf.orig
│   ├── param.conf
│   ├── bbox_example*
│   ├── boundingbox_example_1_0_0_aarch64.eap
│   └── boundingbox_example_1_0_0_LICENSE.txt
```

* **build/package.conf** - Defines the application and its configuration.
* **build/package.conf.orig** - Defines the application and its configuration, original file.
* **build/param.conf** - File containing application parameters.
* **build/bbox_example** - Application executable binary file.
* **build/boundingbox_example_1_0_0_aarch64.eap** - Application package .eap file.
* **build/boundingbox_example_1_0_0_LICENSE.txt** - Copy of LICENSE file.

#### Install your application
Installing your application on an Axis video device is as simple as:

Browse to the following page (replace <axis_device_ip> with the IP number of your Axis video device)

```bash
http://<axis_device_ip>/#settings/apps
```

*Goto your device web page above > Click on the tab **App** in the device GUI > Add **(+)** sign and browse to
the newly built **boundingboxapp_1_0_0_aarch64.eap** > Click **Install** > Run the application by enabling the **Start** switch*

#### The expected output
The boundingboxes will be visible both in liveview and recordings from channel 1 and 2.

When example starts a bounding box is drawn on channel 1 for 10 seconds, then followed by bounding boxes drawn on channel 1 and 2 for another 10 seconds before example finish. Stop/start example application again to repeat.

## License
**[Apache License 2.0](../LICENSE)**