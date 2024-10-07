# syntax=docker/dockerfile:1

ARG UBUNTU_VERSION=22.04

FROM arm64v8/ubuntu:${UBUNTU_VERSION}

RUN DEBIAN_FRONTEND=noninteractive \
    apt-get update && apt-get install -y --no-install-recommends \
    python3 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /opt/app

COPY app/* .
CMD ["python3", "simply_hello.py"]
