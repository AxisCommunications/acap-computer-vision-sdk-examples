## OpenCV QR Decoder
This example shows a minimal QR code detector and decoder application written in Python. 
It involves using OpenCV to create a video stream, NumPy to preprocess
the images and OpenCV to detect and decode any QR codes within the image. The example uses the
[ACAP Computer Vision SDK](https://github.com/AxisCommunications/acap-computer-vision-sdk) to get
OpenCV with VDO integration, allowing capture of a video stream through OpenCV's standard
VideoCapture class. Additionally, Python, NumPy and OpenBLAS are also copied 
from the Computer Vision SDK.

## Prerequisites
The following items are required to run this example:
* Docker Engine version 19.03.5 or higher
* docker-compose version 1.27.4 or higher
* Firmware: 10.5
* ACAP4 running on the camera

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
export SDK_VERSION=1.0-alpha1
export ARCH=armv7hf
export REPO=axisecp
export RUNTIME_IMAGE=arm32v7/ubuntu:20.04

# Local configuration
# To allow retrieval of the image from the cloud
# this should be a repository that you can push to
# and that your camera can pull from, i.e., substitute
# axisecp for your own repository 
export APP_NAME=axisecp/acap-opencv-qr-decoder-python

# Set this to your camera's actual IP
export AXIS_TARGET_IP=192.168.0.90
```

With the environment setup, the application image can be built by running:
 ```
docker build -t $APP_NAME --build-arg SDK_VERSION --build-arg ARCH --build-arg REPO --build-arg RUNTIME_IMAGE --build-arg DOCKER_PROXY=$HTTP_PROXY .
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
**Apache 2.0**

## References
* https://docs.opencv.org/
