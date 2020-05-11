# An inference example application on an edge device
This example is written in Python and implements the following object detection scenarios:
 - Run a video streaming inference on camera
 - Run a still image inference on camera

## Overview
This example composes three different container images into an application that performs object detection using a deep learning model.

The first container contains the actual program being built here, which uses OpenCV to capture pictures from the camera and modifies them to fit the input required by the model. It then uses grpc/protobuf to call the second container, the "inference-server", that performs the actual inference. The inference server implements the TensorFlow Serving API.

Lastly, there is a third container that holds the deep learning model, which is put into a volume that is accessible to the other two images.

## Example Structure
Following are the list of files and a brief description of each file in the example
```bash
inference-client
├── camera-image.yml
├── camera-video.yml
├── detector.py
├── Dockerfile
├── dog416.png
└── README.md
```

* **Dockerfile** - Build Docker image with inference client for camera
* **detector.py** - The inference client main program
* **camera-image.yml** - Docker compose file for still image example
* **camera-video.yml** - Docker compose file for streaming video example
* **dog416.png** - Example image


## How to run the code
These steps shows how to build and run inference-client

Build a Docker inference-client image

 ```
 docker build -t inference-client .
 ```

Push the Docker image to your repo/artifactory server, example:

```
docker tag inference-client axisecp/inference-client:1.0.0-armv7hf
docker push axisecp/inference-client:1.0.0-armv7hf
```
There are two options avialable in this example:

**OPT 1** - Use the following command to run the video streaming inference on the camera, example:
```
docker-compose -H tcp://<IP_Adress:Port> -f camera-video.yml up (e.g., docker-compose -H tcp://172.25.75.141:2375 -f camera-video.yml up)
```
**OPT 2** - Use the following command to run still image inference on the camera, example:
```
docker-compose -H tcp://<IP_Adress:Port> -f camera-image.yml up (e.g., docker-compose -H tcp://172.25.75.141:2375  -f camera-image.yml up)
```

**Note:** To clear inference-client on camera, example:
```
 docker -H tcp://172.25.75.141 system prune -f
 docker -H tcp://172.25.75.141 image rm axisecp/inference-client:0.0.1
```

### The expected output:
`docker-compose -H tcp://172.25.75.141:2375 -f camera-video.yml up`
```
....
object-detector_1           | 1 Objects found
object-detector_1           | person
```

`docker-compose -H tcp://172.25.75.141:2375  -f camera-image.yml up`
```
....
inference-client_1          | 3 Objects found
inference-client_1          | bicycle
inference-client_1          | dog
inference-client_1          | car
```
## License:
Apache 2.0