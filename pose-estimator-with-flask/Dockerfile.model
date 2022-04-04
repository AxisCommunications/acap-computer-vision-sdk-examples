ARG ARCH=armv7hf

FROM arm32v7/alpine as model-image-armv7hf
FROM arm64v8/alpine as model-image-aarch64

FROM model-image-${ARCH}

# Get SSD Mobilenet V2
ADD https://raw.githubusercontent.com/google-coral/test_data/master/movenet_single_pose_lightning_ptq_edgetpu.tflite models/
ADD https://raw.githubusercontent.com/google-coral/test_data/master/movenet_single_pose_lightning_ptq.tflite models/

CMD /bin/ls /models