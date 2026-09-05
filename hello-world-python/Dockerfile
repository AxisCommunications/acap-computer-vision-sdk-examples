# syntax=docker/dockerfile:1

ARG UBUNTU_VERSION=26.04@sha256:2260313b31c8c011cd2eebe728008efac1b3982be73eb71348ea2648d2c0e09b
ARG DEVICE_PLATFORM=linux/arm64/v8

FROM --platform=$DEVICE_PLATFORM ubuntu:${UBUNTU_VERSION}

RUN DEBIAN_FRONTEND=noninteractive \
    apt-get update && apt-get install -y --no-install-recommends \
    python3 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /opt/app

COPY app/* .
CMD ["python3", "simply_hello.py"]
