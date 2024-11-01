# syntax=docker/dockerfile:1

ARG UBUNTU_VERSION=24.04
ARG DEVICE_PLATFORM=linux/arm64/v8

FROM --platform=$DEVICE_PLATFORM ubuntu:${UBUNTU_VERSION}

RUN DEBIAN_FRONTEND=noninteractive \
    apt-get update && apt-get install -y --no-install-recommends \
    python3 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /opt/app

COPY app/* .
CMD ["python3", "simply_hello.py"]
