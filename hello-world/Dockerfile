ARG ARCH=armv7hf
ARG SDK_VERSION=1.0-beta3
ARG REPO=axisecp
ARG RUNTIME_IMAGE=arm32v7/ubuntu:20.04

# Specify which ACAP Computer Vision SDK to use
FROM $REPO/acap-computer-vision-sdk:$SDK_VERSION-$ARCH AS cv-sdk

# Define the runtime image
FROM ${RUNTIME_IMAGE}

# Get the Python package from the CV SDK
COPY --from=cv-sdk /axis/python /

WORKDIR /app
COPY app/* /app
CMD ["python3", "simply_hello.py"]
