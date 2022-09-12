*Copyright (C) 2022, Axis Communications AB, Lund, Sweden. All Rights Reserved.*

# A pose estimator example application on an edge device

This example written in Python implements a pose estimator, that publishes the output using [Flask](https://flask.palletsprojects.com).
The model [MoveNet SinglePose Lightning](https://coral.ai/models/pose-estimation/) takes an 192x192 pixel image as input, and returns the location of 17 keypoints of the human body, with their respective confidence.

![Scheme of the example](assets/scheme.jpg)

## Overview

This example composes three different container images into an application that performs object detection using a deep learning model.

The first container contains the actual program built in this example. It uses [OpenCV](https://opencv.org/) to capture pictures from the camera and modifies them to fit the input required by the model. It then uses [gRPC](https://grpc.io/)/[protobuf](https://developers.google.com/protocol-buffers) to call the second container, the *inference server*, that performs the actual inference. The inference server implements the [TensorFlow Serving API](https://github.com/tensorflow/serving).

Lastly, there is a third container that holds the deep learning model, which is put into a volume that is accessible by the other two images.

### Applications

Pose estimation is useful to accomplish several tasks. For example in action detection it can be applied in:

* autonomous driving, to detect if a pedestrian is waiting or crossing a street
* healthcare, to detect if a person is standing or falling.
Pose estimation models are also considered better than object detection at tracking humans. That is because they can keep tracking the human under partial occlusion, where a classic object detection model is more prone to fail.

### larod-inference server

This image uses the larod service in AXIS OS and the model from the installed model image.
You can find documentation about the larod inference server on the [Machine Learning API documentation page](https://axiscommunications.github.io/acap-documentation/docs/api/native-api.html#machine-learning-api)

The layout of the Docker image containing the model is shown below. The *MODEL_PATH* variable in the configuration file you're using specifies what model to use. By default, the armv7hf configuration file uses the edgetpu model, while the aarch64 configuration file uses the vanilla model.

```text
model
├── movenet_single_pose_lightning_ptq.tflite for CPU and DLPU
└── movenet_single_pose_lightning_ptq_edgetpu.tflite for TPU
```

## Example structure

These are the files used in the example, followed by a brief description of each file:

```text
pose-estimator-with-flask
├── app
│   ├── templates
│   │   └──  index.html
│   └── detector_with_flask.py
├── config
│   ├── env.aarch64
│   └── env.armv7hf
├── docker-compose.yml
├── Dockerfile
├── Dockerfile.model
└── README.md
```

* **app/templates/index.html** - Simple HTML page used to render the video stream
* **app/pose-estimator-with-flask.py** - The inference client main program
* **config/env.aarch64** - Configuration file for Docker Compose to run on aarch64 devices
* **config/env.armv7hf** - Configuration file for Docker Compose to run on armv7hf devices
* **docker-compose.yml** - Docker Compose file for streaming camera video example using larod inference service
* **Dockerfile** - Docker image with inference client for camera
* **Dockerfile.model** - Docker image with inference model

## Requirements

Meet the following requirements to ensure compatibility with the examples:

* Network device
  * Chip: ARTPEC-7 and ARTPEC-8 (e.g., Q1615 Mk III or Q1656)
  * Firmware: 10.9 or higher
  * [Docker ACAP](https://github.com/AxisCommunications/docker-acap) installed and started, using TLS and SD card as storage
* Computer
  * Docker version 20.10.17 or higher
  * Docker Compose version 1.29.2 or higher

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

# Define APP name
export APP_NAME=acap4-pose-estimator-python
export MODEL_NAME=acap-dl-models
docker --tlsverify -H tcp://$AXIS_TARGET_IP:$DOCKER_PORT system prune -af
```

### Build the pose-estimator-with-flask images

```sh
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
docker-compose --tlsverify -H tcp://$AXIS_TARGET_IP:$DOCKER_PORT --env-file ./config/env.$ARCH.$CHIP down -v
```

### The expected output

In your browser, go to http://$AXIS_TARGET_IP:5000 to start the interference. If a person is detected, the key points are drawn on the video stream.

![Pose estimator](assets/frame_36.jpg)

The terminal output shows the key points array and their confidences.

```text
....
pose-estimator_1    | [[0.31955463 0.48752564]
pose-estimator_1    |  [0.2867798  0.5694627 ]
pose-estimator_1    |  [0.2867798  0.5448816 ]
pose-estimator_1    |  [0.31545776 0.5080099 ]
pose-estimator_1    |  [0.3113609  0.4793319 ]
pose-estimator_1    |  [0.35232943 0.46704134]
pose-estimator_1    |  [0.3564263  0.49162248]
pose-estimator_1    |  [0.5162036  0.45065394]
pose-estimator_1    |  [0.36052316 0.4629445 ]
pose-estimator_1    |  [0.5243973  0.5243973 ]
pose-estimator_1    |  [0.53259104 0.53259104]
pose-estimator_1    |  [0.61043125 0.43426654]
pose-estimator_1    |  [0.5448816  0.52030045]
pose-estimator_1    |  [0.70056206 0.43426654]
pose-estimator_1    |  [0.8808236  0.3564263 ]
pose-estimator_1    |  [0.9135984  0.36871687]
pose-estimator_1    |  [0.89311415 0.38510427]]
pose-estimator_1    | [0.02458112 0.02048427 0.02048427 0.05735596 0.02048427 0.05735596
pose-estimator_1    |  0.05735596 0.01638742 0.03687169 0.01229056 0.02048427 0.02458112
pose-estimator_1    |  0.02048427 0.01638742 0.15568045 0.07374337 0.05735596]
```

### Hardware acceleration

The ./config folder contains configuration files with the parameters to run the inference on different camera models. The parameters also gives you the possibility to use the hardware accelerator.
To achieve the best performance we recommend using:

* the TPU (Tensor Processing Unit) equipped with artpec7 cameras (for example [Axis-Q1615 Mk III](https://www.axis.com/products/axis-q1615-mk-iii))
* or the DLPU (Deep Learning Processing Unit) equipped in artpec8 cameras (e.g. [Axis-Q1656](https://www.axis.com/products/axis-q1656))

## License

**[Apache License 2.0](../LICENSE)**
