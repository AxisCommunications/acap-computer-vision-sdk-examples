ARG ARCH=armv7hf
ARG REPO=axisecp
ARG SDK_VERSION=1.3
ARG UBUNTU_VERSION=22.04

FROM arm32v7/ubuntu:${UBUNTU_VERSION} as runtime-image-armv7hf
FROM arm64v8/ubuntu:${UBUNTU_VERSION} as runtime-image-aarch64

FROM ${REPO}/acap-computer-vision-sdk:${SDK_VERSION}-${ARCH}-runtime AS cv-sdk
FROM runtime-image-${ARCH}

COPY --from=cv-sdk /axis/opencv /
COPY --from=cv-sdk /axis/openblas /
COPY --from=cv-sdk /axis/python /
COPY --from=cv-sdk /axis/python-numpy /

WORKDIR /app
COPY app/qr.py .
CMD ["python3", "qr.py"]
