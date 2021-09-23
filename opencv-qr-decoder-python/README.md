*Copyright (C) 2021, Axis Communications AB, Lund, Sweden. All Rights Reserved.*

## OpenCV QR Decoder
This example shows a minimal QR code detector and decoder application written in Python.
It involves using OpenCV to create a video stream, NumPy to preprocess
the images and OpenCV to detect and decode any QR codes within the image. The example uses the
[ACAP Computer Vision SDK](https://github.com/AxisCommunications/acap-computer-vision-sdk) to get
OpenCV with VDO integration, allowing capture of a video stream through OpenCV's standard
VideoCapture class. Additionally, Python, NumPy and OpenBLAS are also copied
from the Computer Vision SDK.

## Requirements
To ensure compatibility with the examples, the following requirements shall be met:
* Camera: ARTPEC-7 DLPU devices (e.g., Q1615 MkIII)
* docker-compose version 1.29 or higher
* Docker version 20.10.8 or higher
* Firmware: 10.7
* docker-acap installed on the camera
* docker-acap set to use external memory card

## Application layout
```bash
opencv-qr-decoder-python
├── app
│   └── qr.py
├── docker-compose.yml
├── Dockerfile
└── README.md
```
* **qr.py**              - The application's main script
* **Dockerfile**         - Dockerfile specifying how the application runtime is built
* **docker-compose.yml** - docker-compose file specifying how/with what settings the application is started

### Running the application
Begin by setting up some environment variables.
Below, we define the camera's IP, the desired app name and the path and version of the ACAP Computer Vision SDK.
```
# CV SDK configuration
export ARCH=armv7hf
export REPO=axisecp
export RUNTIME_IMAGE=arm32v7/ubuntu:20.04

# Local configuration
# To allow retrieval of the image from the cloud
# this should be a repository that you can push to
# and that your camera can pull from, i.e., substitute
# axisecp for your own repository
export APP_NAME=axisecp/acap-opencv-qr-decoder-python

# Set your camera IP address
export AXIS_TARGET_IP=<actual camera IP address>
```

With the environment setup, the application image can be built by running:
 ```
docker build -t $APP_NAME --build-arg ARCH --build-arg REPO --build-arg RUNTIME_IMAGE --build-arg DOCKER_PROXY=$HTTP_PROXY .
 ```

To run the image, it needs to be available to the camera. This is either done by pushing it
to a container registry and pulling it from there (option 1), or saving it to a compressed
`.tar` file and uploading it directly to the camera (option 2).

Option 1: Pushing the image to cloud and pulling it to the camera
```
docker push $APP_NAME
docker -H tcp://$AXIS_TARGET_IP pull $APP_NAME
```

Option 2: Saving the image to file and uploading directly to the camera
```
docker save $APP_NAME -o app.tar
docker -H tcp://$AXIS_TARGET_IP load -i app.tar
```

Once the application image is available to the camera, running it is as easy as:
```
docker-compose -H tcp://$AXIS_TARGET_IP:2375 up
```

## License
**[Apache License 2.0](../LICENSE)**


## References
* https://docs.opencv.org/
