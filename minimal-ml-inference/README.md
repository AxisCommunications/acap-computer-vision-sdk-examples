*Copyright (C) 2022, Axis Communications AB, Lund, Sweden. All Rights Reserved.*


# A minimal, machine learning inference application
This example demonstrates how to create a simple Python application that performs inference on the camera image. This is done using the ACAP Computer Vision SDK.

The example largely consists of three parts: the Python script [simple_inference.py](simple_inference.py) that is run on device, the [Dockerfile](Dockerfile) which build the application image and the [docker-compose.yml](docker-compose.yml) which specifies how the inference server and client is connected as well as specify the required mounts and environment variables needed.

## Example structure
Following are the list of files and a brief description of each file in the example
```bash
minimal-ml-inference
├── config
│   ├── env.aarch64
│   └── env.armv7hf
├── simple_inference.py
├── Dockerfile
├── Dockerfile.model
├── docker-compose.yml
└── README.md
```

* **config/*** - Environment configuration files
* **simple_inference.py** - A Python script that captures an image and send an inference call to the model server
* **Dockerfile** - Build instructions for the application image that is run on the camera
* **Dockerfile.model** - Build instructions for the inference model
* **docker-compose.yml** - A docker-compose file that specifies how the application is run (mounts, environment variables, etc.,)

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
# Valid options for chip on armv7hf are 'tpu' (hardware accelerator) or 'cpu'
export CHIP=tpu
```
```sh
# For arm64
export ARCH=aarch64
# Valid options for chip on aarch64 are 'artpec8' (hardware accelerator) or 'cpu'
export CHIP=artpec8 
```

### Set your camera IP address and clear Docker memory
```sh
export AXIS_TARGET_IP=<actual camera IP address>
export DOCKER_PORT=2376
docker --tlsverify -H tcp://$AXIS_TARGET_IP:$DOCKER_PORT system prune -af
```

### Build the minimal-ml-inference images
```sh
# Define APP name
export APP_NAME=acap4-minimal-ml-inference
export MODEL_NAME=acap-dl-models

# Install qemu to allow build flask for a different architecture
docker run -it --rm --privileged multiarch/qemu-user-static --credential yes --persistent yes

# Build and upload inference client for camera
docker build . -t $APP_NAME --build-arg ARCH 
docker save $APP_NAME | docker --tlsverify -H tcp://$AXIS_TARGET_IP:$DOCKER_PORT load

# Build and upload inference models
docker build . -f Dockerfile.model -t $MODEL_NAME --build-arg ARCH
docker save $MODEL_NAME | docker --tlsverify -H tcp://$AXIS_TARGET_IP:$DOCKER_PORT load

# Use the following command to run the example on the camera
docker-compose --tlsverify -H tcp://$AXIS_TARGET_IP:$DOCKER_PORT --env-file ./config/env.$ARCH.$CHIP up

# Terminate with Ctrl-C and cleanup
docker-compose --tlsverify -H tcp://$AXIS_TARGET_IP:$DOCKER_PORT down -v
```

The expected output from the application is the raw predictions from the model specified in the environment variable.

### Hardware acceleration
The ./config folder contains configuration files with the parameters to run the inference on different camera models, also giving the possibility to use the hardware accelerator. 
To achieve the best performance we recommend using the TPU (Tensor Processing Unit) equipped with artpec7 cameras (e.g. [Axis-Q1615 Mk III](https://www.axis.com/products/axis-q1615-mk-iii))
or the DLPU (Deep Learning Processing Unit) equipped in artpec8 cameras (e.g. [Axis-Q1656](https://www.axis.com/products/axis-q1656))


## License
**[Apache License 2.0](../LICENSE)**