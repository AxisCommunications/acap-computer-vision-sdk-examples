*Copyright (C) 2021, Axis Communications AB, Lund, Sweden. All Rights Reserved.*

 # How to use Web Server in ACAP 4
This document explains briefly how to build and use [Monkey Web Server](https://github.com/monkey/monkey) in ACAP4. Monkey is a fast and lightweight Web Server for Linux. It has been designed to be very scalable with low memory and CPU consumption, the perfect solution for Embedded Linux and high end production environments. Besides the common features as HTTP server, it expose a flexible C API which aims to behave as a fully HTTP development framework, so it can be extended as desired through the plugins interface. The Monkey Web Server [documentation](http://monkey-project.com/documentation/1.5) describes the configuration in detail.

## Structure of this application
Below is the structure of the application with a brief description of its files.
```sh
web-server
├── Dockerfile - Specifications on how to build the camera docker image
└── README.md - Instructions on how to build and run the application
```

## Requirements
To ensure compatibility with the examples, the following requirements shall be met:
* Camera: ARTPEC-{7-8} DLPU devices (e.g., Q1615 MkIII)
* docker-compose version 1.29 or higher
* Docker version 20.10.8 or higher
* Firmware: 10.9
* docker-acap installed on the camera
* docker-acap set to use TLS and external memory card

## Limitations
* Apache Reverse Proxy can not translate content with absolute addresses (i.e. /image.png) in the HTML page. Use only relative content (i.e. image.png or ../image.png)). More information how to handle relative urls correctly with a reverse proxy [here](https://serverfault.com/questions/561892/how-to-handle-relative-urls-correctly-with-a-reverse-proxy).

## Configure Camera Apache Web Server to forward web requests
The Web Server can be accessed from a Web Browser eighter directly using a port number (i.e. http://mycamera:8080) or through the Apache Server in the camera using an extension to the camera web URL (i.e http://mycamera/monkey). To configure the Apache Server as a Reverse Proxy Server, use the procedure shown below.
```sh
# Do ssh login to the camera
ssh root@<CAMERA_IP>

# Add Reverse Proxy configuration to the Apache Server, example:
cat >> /etc/apache2/httpd.conf <<EOF
ProxyPass /monkey/demo http://localhost:2001
ProxyPassReverse /monkey/demo http://localhost:2001
ProxyPass /monkey http://localhost:8080
ProxyPassReverse /monkey http://localhost:8080
EOF

# Restart the Apache Server
systemctl restart httpd
```

## Build and run the Web Server
Start by building the image containing the Web Server code with examples. This will compile the code to an executable and create an armv7hf container containing the executable, which can be uploaded to and run on the camera. After the Web Server is started it can be accessed from a web browser by specifying the web address: http://mycamera/monkey/ or http://mycamera:8080
```sh
# Set your camera IP address and clear docker memory
export AXIS_TARGET_IP=<actual camera IP address>
export DOCKER_PORT=2375
docker --tlsverify -H tcp://$AXIS_TARGET_IP:$DOCKER_PORT system prune -af
```
### Export environment variables for arm32 cameras
```sh
# Set environment variables
# ARCH defines what architecture to use (e.g., armv7hf, aarch64)
# RUNTIME_IMAGE defines what base image should be used for the application image 


export ARCH=armv7hf
export RUNTIME_IMAGE=arm32v7/ubuntu:20.04
export APP_NAME=monkey
```
### Export environment variables for arm64 cameras
```sh
export ARCH=aarch64
export RUNTIME_IMAGE=arm64v8/ubuntu:20.04
export APP_NAME=monkey

```


# Build the application and load on camera
```sh
docker build . -t $APP_NAME --build-arg ARCH --build-arg RUNTIME_IMAGE
docker save $APP_NAME | docker --tlsverify -H tcp://$AXIS_TARGET_IP:$DOCKER_PORT  load
```
# Start Web Server on the camera
```sh
docker --tlsverify  -H tcp://$AXIS_TARGET_IP:$DOCKER_PORT run --rm -p 8080:80 -it $APP_NAME
```

### The expected output
Running the application on the camera should output information similar to what's shown below.
```sh
Monkey HTTP Server v1.5.6
Built : Jun 18 2021 11:05:42 (arm-linux-gnueabihf-gcc -mthumb -mfpu=neon -mfloat-abi=hard -mcpu=cortex-a9 9.3.0)
Home  : http://monkey-project.com
[+] Process ID is 6
[+] Server socket listening on Port 80
[+] 2 threads, 253 client connections per thread, total 506
[+] Transport layer by liana in http mode
[+] Linux Features: TCP_FASTOPEN SO_REUSEPORT
```

## C API Examples
Some C API examples are included in the Web Server container that has been built. The commands below show how to run the examples on the camera. To see the result, use a web browser and web address: http://mycamera/monkey/demo/ or http://mycamera:2001
```sh
# Run the hello example
docker --tlsverify -H tcp://$AXIS_TARGET_IP:$DOCKER_PORT  run --rm -p 2001:2001 -it $APP_NAME hello

# Run the list directory example
docker --tlsverify -H tcp://$AXIS_TARGET_IP:$DOCKER_PORT  run --rm -p 2001:2001 -it $APP_NAME list

# Run the quiz example
docker --tlsverify -H tcp://$AXIS_TARGET_IP:$DOCKER_PORT  run --rm -p 2001:2001 -it $APP_NAME quiz
```

## Proxy settings
Depending on the network, you might need proxy settings in the following file: *~/.docker/config.json.*

For reference please see: https://docs.docker.com/network/proxy/.
```

## License
**[Apache License 2.0](../LICENSE)**
