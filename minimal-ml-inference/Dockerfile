# syntax=docker/dockerfile:1

ARG REPO=axisecp
ARG SDK_VERSION=1.15
ARG UBUNTU_VERSION=22.04

# Specify which ACAP Computer Vision SDK to use
FROM ${REPO}/acap-computer-vision-sdk:${SDK_VERSION}-aarch64 AS cv-sdk

# Define the runtime image
FROM arm64v8/ubuntu:${UBUNTU_VERSION}

# Get packages from the CV SDK
COPY --from=cv-sdk /axis/python /
COPY --from=cv-sdk /axis/python-tfserving /
COPY --from=cv-sdk /axis/python-vdoproto /
COPY --from=cv-sdk /axis/python-numpy /
COPY --from=cv-sdk /axis/opencv /
COPY --from=cv-sdk /axis/openblas /

# Copy the application script to the container
WORKDIR /app
COPY simple_inference.py .
CMD ["python3", "simple_inference.py"]
