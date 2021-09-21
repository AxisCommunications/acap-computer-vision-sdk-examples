ARG ARCH=armv7hf
ARG SDK_VERSION=1.0-beta2
ARG REPO=axisecp
ARG RUNTIME_IMAGE=arm32v7/ubuntu:20.04

FROM $REPO/acap-computer-vision-sdk:$SDK_VERSION-$ARCH AS cv-sdk
FROM ${RUNTIME_IMAGE}
COPY --from=cv-sdk /axis/python /
COPY --from=cv-sdk /axis/python-numpy /
COPY --from=cv-sdk /axis/python-tfserving /
COPY --from=cv-sdk /axis/opencv /
COPY --from=cv-sdk /axis/openblas /

WORKDIR /app
COPY app/* /app
CMD ["python3", "detector.py"]
