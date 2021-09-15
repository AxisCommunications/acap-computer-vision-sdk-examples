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

## Prerequisites
The following items are required to run this example:
* Camera: Q1615-MkIII
* ACAP4 Docker version 19.03.5 or higher
* Camera Firmware: 10.2

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
# Set your camera IP address
export AXIS_TARGET_IP=<actual camera IP address>

# To allow retrieval of the image from the cloud
# this should be a repository that you can push to
# and that your camera can pull from, i.e., substitute
# axisecp for your own repository
export APP=axisecp/monkey

docker build . --build-arg http_proxy --build-arg https_proxy -t $APP
docker push $APP

# Start Web Server on the camera
docker -H tcp://$AXIS_TARGET_IP system prune -af
docker -H tcp://$AXIS_TARGET_IP run --rm -p 8080:80 -it $APP
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
docker -H tcp://$AXIS_TARGET_IP run --rm -p 2001:2001 -it $APP hello

# Run the list directory example
docker -H tcp://$AXIS_TARGET_IP run --rm -p 2001:2001 -it $APP list

# Run the quiz example
docker -H tcp://$AXIS_TARGET_IP run --rm -p 2001:2001 -it $APP quiz
```

## Proxy settings
Depending on the network, you might need proxy settings in the following file: *~/.docker/config.json.*

For reference please see: https://docs.docker.com/network/proxy/.

*Proxy settings can also be added to the edge device:*
```sh
ssh root@<CAMERA_IP>
```

## License
**[Apache License 2.0](../LICENSE)**
