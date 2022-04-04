ARG ARCH=armv7hf

FROM arm32v7/alpine as model-image-armv7hf
FROM arm64v8/alpine as model-image-aarch64

FROM model-image-${ARCH}

# Get SSD Mobilenet V2
ADD https://github.com/google-coral/edgetpu/raw/master/test_data/ssd_mobilenet_v2_coco_quant_postprocess_edgetpu.tflite models/
ADD https://github.com/google-coral/edgetpu/raw/master/test_data/ssd_mobilenet_v2_coco_quant_postprocess.tflite models/
ADD https://github.com/google-coral/edgetpu/raw/master/test_data/coco_labels.txt models/
ADD https://github.com/google-coral/edgetpu/raw/master/test_data/COPYRIGHT models/

CMD /bin/ls /models