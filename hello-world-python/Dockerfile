# syntax=docker/dockerfile:1

ARG REPO=axisecp
ARG SDK_VERSION=1.15
ARG UBUNTU_VERSION=22.04


FROM ${REPO}/acap-computer-vision-sdk:${SDK_VERSION}-aarch64 AS cv-sdk

FROM arm64v8/ubuntu:${UBUNTU_VERSION}

# Get the Python package from the CV SDK
COPY --from=cv-sdk /axis/python /

WORKDIR /app
COPY app/* /app
CMD ["python3", "simply_hello.py"]
