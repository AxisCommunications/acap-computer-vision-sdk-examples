*Copyright (C) 2021, Axis Communications AB, Lund, Sweden. All Rights Reserved.*


# A minimal Python application
This example demonstrates how to create a simple Python application using the ACAP Computer Vision SDK and run it on an edge device.

Going from zero to a Python application running on an AXIS device is quite easy. First, the application script is written, as in the hello-world script in [app/simply_hello.py](app/simply_hello.py). Next, the [Dockerfile](Dockerfile) which build the application image is constructed. This needs to pull in packages from the ACAP Computer Vision SDK, as is done using the `COPY` commands. Finally, the application needs to be built and uploaded, as is specified below.

## Example structure
Following are the list of files and a brief description of each file in the example
```bash
hello-world
├── app
├── |- simply_hello.py
├── Dockerfile
├── docker-compose.yml
└── README.md
```

* **simply_hello.py** - A Python script that prints "Hello World"
* **Dockerfile** - Build instructions for the application image that is run on the camera
* **docker-compose.yml** - A docker-compose file that specifies how the application is run

## Requirements
To ensure compatibility with the examples, the following requirements shall be met:
* Camera: ARTPEC-{7-8} DLPU devices (e.g., Q1615 MkIII)
* docker-compose version 1.29 or higher
* Docker version 20.10.8 or higher
* Firmware: 10.9
* [Docker ACAP](https://github.com/AxisCommunications/docker-acap) installed and started, using TLS and SD card as storage

## How to run the code
### Export the environment variable for the architecture 
Export the ARCH variable depending on the architecture of your camera
```sh
# For arm32
export ARCH=armv7hf
# For arm64
export ARCH=aarch64
```

### Set your camera IP address define APP name and clear Docker memory 
```sh
# Set camera IP
export AXIS_TARGET_IP=<actual camera IP address>
export DOCKER_PORT=2376

# Define APP name
export APP_NAME=hello-world

# Clean docker memory
docker --tlsverify -H tcp://$AXIS_TARGET_IP:$DOCKER_PORT system prune -af
```

### Build and run the images
With the environment setup, the `hello-world` image can be built. The environment variables are supplied as build arguments such that they are made available to docker during the build process:

```sh
docker build . -t $APP_NAME --build-arg ARCH
```

Next, the built image needs to be uploaded to the device. This can be done through a registry or directly. In this case, the direct transfer is used by piping the compressed application directly to the device's docker client:

```sh
docker save $APP_NAME | docker --tlsverify -H tcp://$AXIS_TARGET_IP:$DOCKER_PORT  load
```

With the application image on the device, it can be started. As this example does not use e.g., OpenCV, no special mounts are needed, making the `docker-compose.yml` file very simple:

```sh
docker-compose --tlsverify -H tcp://$AXIS_TARGET_IP:$DOCKER_PORT up

# Cleanup after execution
docker-compose --tlsverify -H tcp://$AXIS_TARGET_IP:$DOCKER_PORT down -v
```

The expected output from the application is simply:

```sh
Hello World!
```
## License
**[Apache License 2.0](../LICENSE)**
