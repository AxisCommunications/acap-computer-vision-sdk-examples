ARG ARCH=armv7hf
ARG SDK_VERSION=1.2
ARG REPO=axisecp

FROM arm32v7/ubuntu:20.04 as runtime-image-armv7hf
FROM arm64v8/ubuntu:20.04 as runtime-image-aarch64

FROM $REPO/acap-computer-vision-sdk:$SDK_VERSION-$ARCH AS cv-sdk
FROM runtime-image-${ARCH}

# Get the Python package from the CV SDK
COPY --from=cv-sdk /axis/python /

WORKDIR /app
COPY app/* /app
CMD ["python3", "simply_hello.py"]
