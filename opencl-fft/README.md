# How to use OpenCL in ACAP 4
This example explains briefly how to use OpenCL in ACAP4. The example code application is written in C++ and OpenCL. The supplied OpenCL application performs hardware accelerated Fast Fourier Transform (FFT).

The example code performs its calculations on the GPU, which allows faster computation as compared to performing the same operations on the CPU. While FFT is not necessarily the most common operation to perform on a camera, any OpenCL application can be run in this way, which allows fast execution of e.g., image processing operations.

## Structure of this example
Below is the structure of the example with a brief description of its files.
```sh
opencl-fft
 |- app - directory holding the OpenCL application files
 | |- cl_test.cpp - The main OpenCL application file
 | |- clutil.cpp - Utility functions for the OpenCL application
 | |- clutil.h - Headers for the utility functions
 | |- fft.cl - The OpenCL code for the application
 |- Dockerfile - Specifications on how to build the camera image
 |- README.md - Instructions on how to run the application
 |- sources.list - Additional sources for retrieving armhf packages when building image
```

## Prerequisites
The following items are required to run this example:
* Camera: Q1615-MkIII
* Docker version 19.03.5 or higher
* Firmware: 10.5
* ACAP4 installed on the camera

## Running the example
1. Start by building the image OpenCL containing the OpenCL code. This will compile the OpenCL code to an executable and create an armv7hf container containing the executable, which can be uploaded to and run on the camera.
```sh
# Find build folder
cd acap-application-examples/opencl-fft

# Set your camera IP address
export AXIS_TARGET_IP=<actual camera IP address>

# Adjust some environment variables to your preference and build the application image
export REPO=axisecp
export SDK_VERSION=1.0-alpha1
export ARCH=armv7hf
export UBUNTU_VERSION=20.04
export RUNTIME_IMAGE=arm32v7/ubuntu:20.04

# To allow retrieval of the image from the cloud
# this should be a repository that you can push to
# and that your camera can pull from, i.e., substitute
# axisecp for your own repository 
export APP_NAME=axisecp/acap-opencl-fft

docker build . --tag $APP_NAME --build-arg DOCKER_PROXY=$http_proxy \
--build-arg REPO --build-arg SDK_VERSION --build-arg ARCH --build-arg UBUNTU_VERSION
```

The `APP_NAME` variable is the name you want to tag the image with, e.g., `my-opencl-app:1.0`. `AXIS_TARGET_IP` should be set to the IP address of the camera running ACAP4. Additionally, the environment variable `APP_COMMAND` may be specified as a positive integer. It refers to the number of iterations of FFT to run.

2. Getting the image to run on a camera can be done e.g., by pushing it to a registry as seen below. The `-H` flag redirects the command to a remote Docker daemon at the specified IP adress. Push the image to the registry and pull it from the registry to the camera by executing:
```sh
docker push $APP_NAME

# Clear docker memory on camera
docker -H tcp://$AXIS_TARGET_IP system prune -af

# Load application image on camera
docker -H tcp://$AXIS_TARGET_IP pull $APP_NAME
```

3. Once the image is on the camera, it can be started e.g., through docker-compose. The `docker-compose.yml` file also contains the dependencies that are needed to run OpenCL on this setup. Start the container on the camera by running:
```sh
docker-compose -H tcp://$AXIS_TARGET_IP:2375 up
```

### The expected output
Running the example on the camera should output FFT-related information similar to what's shown below.
```sh
Creating network "opencl-fft_default" with the default driver
Creating opencl-fft_opencl-fft_1 ... done
Attaching to opencl-fft_opencl-fft_1
opencl-fft_1  | OpenCL FFT test starting
opencl-fft_1  | Running FFT length 32768, 1 time(s)
opencl-fft_1  | Initializing device(s)...
opencl-fft_1  | Creating context
opencl-fft_1  | Created context
opencl-fft_1  | Initialized device(s)
opencl-fft_1  | Get the Device info and select Device...
opencl-fft_1  | # of Devices Available = 1
opencl-fft_1  | # of Compute Units = 1
opencl-fft_1  | # compute units = 1
opencl-fft_1  | Creating Command Queue...
(...)
opencl-fft_1  | Successfully completed length 32768
opencl-fft_opencl-fft_1 exited with code 0
```
## Proxy settings
Depending on the network, you might need proxy settings in the following file: *~/.docker/config.json.*

For reference please see: https://docs.docker.com/network/proxy/.

*Proxy settings can also be added to the edge device:*
```sh
ssh root@$CAMERA_IP
```
