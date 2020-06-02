# An openCV based example application on an edge device
This readme file explains how to build an openCV application.
It is achived by using the ACAP API image containing "OpenCV SDK" that
contains all the essential parts needed. There are tools for building
an application and running the container image on an edge device.

OpenCV v4.2.0 is used and comes with following modules:
core, imgproc, imgcodecs, videoio,objdetect, python3, video.

Together with this file you should be able to find a directory called
app, that directory contains the "capture.cpp" application which can
easily be compiled and run with the help of the tools and step by step
below.

## Getting started
These instructions below will guide you on how to execute the code. Below is the structure and scripts used in the example:

```bash
opencv-capture-app
├── app
│   ├── Makefile
│   ├── Dockerfile
│   ├── sources.list
│   ├── build.sh
|   ├── upload.sh
│   └── src
│       └── capture.cpp
├── docker-compose.yml
└── README.md
```
* **capture.cpp**        - Example application to capture camera properties such as time stamps, zoom, focus etc.
* **dockerfile**         - Docker file with the toolchain included to run the example.
* **docker-compose.yml** - Docker compose file contains the lattest image of the example from dockerhub.
* **README.md**          - Step by step instructions on how to run the example.
* **build.sh**           - Builds and tags the image of objdetect.cpp image e.g., axisecp/acap4-object-detector-cpp:1.0.0-rc.1
* **upload.sh**          - To push/pull images to the camera with the Docker Daemon ACAP installed and <APP_IMAGE>.

### Limitations
* OpenCV module choice cannot be made in this version.
* In order to change the binary name it has to be done in the Makefile

### How to run the code
Below is a step by step on the whole process. So basically starting
with the generation of the images to run the final application on an
edge device.

If you would like to build your own app, just add your files under the source folder and they will be compiled against the openCV SDK just by running the following command:

 ```
./build.sh <APP_IMAGE>
 ```
*Note:* In some cases, proxy settings on your host machine and the edge device need to be added to be able to pull and push images. Depending on if there is a proxy server setup
on the network you are running on. Please see [Proxy settings](#proxy-settings)

There are two options to run the capture app.

#### [opt 1] Save the image as tarball
*NOTE:* This option is perhaps not recommended, it's better to involve a registry e.g docker hub, if there is not a strong need for this. Otherwise, please continue to [opt 2].

Save the image to a tarball is one option the other option would be
to push to a docker repo. Which is described further below.

Install DockerD ACAP prior to running this command
```
docker save -o <application_tarball_name>.tar <opencv_target_application_image>
```
#### [opt 1] Load the image if it's a tarball
```
docker -H tcp://<IP> load -i opencv_img.tar
```
#### [opt 2] push it to a repo/artifactory server.
```
docker push axisecp/opencv-application-examples
```
#### [opt 2] pull it from docker hub:
```
docker -H tcp://<IP> pull axisecp/opencv-application-examples
(e.g., docker -H tcp://172.25.75.38 pull axisecp/opencv-application-examples)
```
#### Run the container
```
docker-compose -H tcp://<IP:Port> -f docker-compose.yml up
(e.g., docker-compose -H  tcp://172.25.75.38:2375 -f docker-compose.yml up)
```
#### The expected output:
```bash
FRAME[ 0] 640x360 fd[7]  offs[0xc1000] size[0x56400]
FRAME[ 1] 640x360 fd[11] offs[0xc3000] size[0x56400]
FRAME[ 2] 640x360 fd[7]  offs[0xc5000] size[0x56400]
FRAME[ 3] 640x360 fd[11] offs[0xc1000] size[0x56400]
FRAME[ 4] 640x360 fd[7]  offs[0xc3000] size[0x56400]
FRAME[ 5] 640x360 fd[11] offs[0xc5000] size[0x56400]
FRAME[ 6] 640x360 fd[7]  offs[0xc1000] size[0x56400]
FRAME[ 7] 640x360 fd[11] offs[0xc3000] size[0x56400]
FRAME[ 8] 640x360 fd[7]  offs[0xc5000] size[0x56400]
FRAME[ 9] 640x360 fd[11] offs[0xc1000] size[0x56400]
FRAME[10] 640x360 fd[7]  offs[0xc3000] size[0x56400]
FRAME[11] 640x360 fd[11] offs[0xc5000] size[0x56400]
FRAME[12] 640x360 fd[7]  offs[0xc1000] size[0x56400]
FRAME[13] 640x360 fd[11] offs[0xc3000] size[0x56400]
FRAME[14] 640x360 fd[7]  offs[0xc5000] size[0x56400]
FRAME[15] 640x360 fd[11] offs[0xc1000] size[0x56400]
FRAME[16] 640x360 fd[7]  offs[0xc3000] size[0x56400]
FRAME[17] 640x360 fd[11] offs[0xc5000] size[0x56400]
FRAME[18] 640x360 fd[7]  offs[0xc1000] size[0x56400]
FRAME[19] 640x360 fd[11] offs[0xc3000] size[0x56400]
FRAME[20] 640x360 fd[7]  offs[0xc5000] size[0x56400]
FRAME[21] 640x360 fd[11] offs[0xc1000] size[0x56400]
FRAME[22] 640x360 fd[7]  offs[0xc3000] size[0x56400]
FRAME[23] 640x360 fd[11] offs[0xc5000] size[0x56400]
FRAME[24] 640x360 fd[7]  offs[0xc1000] size[0x56400]
FRAME[25] 640x360 fd[11] offs[0xc3000] size[0x56400]
FRAME[26] 640x360 fd[7]  offs[0xc5000] size[0x56400]
FRAME[27] 640x360 fd[11] offs[0xc1000] size[0x56400]
FRAME[28] 640x360 fd[7]  offs[0xc3000] size[0x56400]
FRAME[29] 640x360 fd[11] offs[0xc5000] size[0x56400]
Captured 30 frames in 973831µs
iteration[0] gain: 191.000000   expo: 503.000000   zoom: 1.862427   focus: 0.902723   f-number: 1.718091
iteration[1] gain: 191.000000   expo: 503.000000   zoom: 1.862427   focus: 0.902723   f-number: 1.718091
iteration[2] gain: 191.000000   expo: 503.000000   zoom: 1.862427   focus: 0.902723   f-number: 1.718091
Queried settings 3 times in 15344µs
```
#### OpenCV-VideoIO UniMatrix extensions

These extensions facilitate inter-process zero-copy by allowing buffers
to be forwarded across unix domain sockets.

Mandatory zero-copy extensions:
* CAP_PROP_UNIMATRIX_FD
  Buffer file descriptor
* CAP_PROP_UNIMATRIX_FD_OFFSET
  Buffer offset
* CAP_PROP_UNIMATRIX_FD_CAPACITY
  Buffer capacity
* CAP_PROP_UNIMATRIX_MAX_BUFFERS
  Maximum buffers in-flight

UniMatrix conforming backends must support at least *one* of these fourcc formats:
* NV12
* NV21
* RGB3

The most efficient color format should always be default.

UniMatrix conforming backends must support monochrome fourcc:
* Y800

Optional properties:
* CAP_PROP_UNIMATRIX_FNUMBER
  f-number

#### OpenCV-VideoIO Axis implementation

Axis supports all UniMatrix extensions:
* CAP_PROP_UNIMATRIX_FD
* CAP_PROP_UNIMATRIX_FD_OFFSET
* CAP_PROP_UNIMATRIX_FD_CAPACITY
* CAP_PROP_UNIMATRIX_MAX_BUFFERS
* CAP_PROP_UNIMATRIX_FNUMBER

These UniMatrix stream formats are not supported:
* NV21

Below stream formats are fully hardware-accelerated:
* Y800
* NV12
  This is the default native format of the camera.

Below stream formats are converted from the native format (i.e. it has a performance penalty):
* RGB3

These *stream* properties can be changed before capturing the first frame,
not when the stream is running:
* CAP_PROP_FPS
* CAP_PROP_FOURCC
* CAP_PROP_CHANNEL
* CAP_PROP_FRAME_WIDTH
* CAP_PROP_FRAME_HEIGHT

These *stream* properties are read-only:
* CAP_PROP_POS_MSEC
* CAP_PROP_POS_FRAMES

These *image* properties are read-only:
* CAP_PROP_ZOOM
  Zoom factor (1.0-)
* CAP_PROP_FOCUS
  Focus dioptre
* CAP_PROP_GAIN
  Gain in milli-dB
* CAP_PROP_EXPOSURE
  Exposure in µs
* CAP_PROP_UNIMATRIX_FNUMBER
  f-number

## Proxy settings
Depending on the network you are connected to.
The file that needs those settings is: *~/.docker/config.json.* For reference please see: https://docs.docker.com/network/proxy/.

*Proxy settings can also be added on the edge device*
```
ssh root@<device_ip>
```
* **Run on the device:**

```
  cat >> /etc/systemd/system/sdkrun_dockerd.service <<EOF
  [Service]
  Environment="HTTP_PROXY=http://<myproxy.com>:<port>"
  Environment="HTTPS_PROXY=http://<myproxy>:<port>"
  Environment="NO_PROXY=localhost,127.0.0.0/8,10.0.0.0/8,192.168.0.0/16,172.16.0.0/12,.<domain>"
  EOF
```
* Then restart the dockerd acap
```
  systemctl daemon-reload
  systemctl restart sdkrun_dockerd
```
## License
**Apache 2.0**

## References
* https://docs.opencv.org/4.2.0/
