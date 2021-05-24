# Specify the architecture at build time: mipsis32r2el/armv7hf/aarch64
# Should be used for getting API image
# Currently, only armv7hf is supported
ARG ARCH=armv7hf
ARG REPO=axisecp
ARG SDK_VERSION=1.0-alpha1
ARG RUNTIME_IMAGE=arm32v7/ubuntu:20.04

FROM ${REPO}/acap-computer-vision-sdk:${SDK_VERSION}-${ARCH}-runtime AS cv-sdk-runtime
FROM ${REPO}/acap-computer-vision-sdk:${SDK_VERSION}-${ARCH}-devel AS cv-sdk-devel

# Setup proxy configuration
ARG DOCKER_PROXY
ENV http_proxy=$DOCKER_PROXY
ENV https_proxy=$DOCKER_PROXY
ENV HTTP_PROXY=$DOCKER_PROXY
ENV HTTPS_PROXY=$DOCKER_PROXY

ENV DEBIAN_FRONTEND=noninteractive

## Install dependencies
RUN apt-get update && apt-get install -y -f \
    g++-arm-linux-gnueabihf \
    make\
    pkg-config \
    libglib2.0-dev \
    libsystemd0

RUN dpkg --add-architecture armhf &&\
    apt-get update && apt-get install -y  \
        libglib2.0-dev:armhf \
        libsystemd0:armhf

COPY app/Makefile /build/
COPY app/src/ /build/src/
WORKDIR /build
RUN make

FROM ${RUNTIME_IMAGE}
COPY --from=cv-sdk-devel /build/capture_app /usr/bin/
COPY --from=cv-sdk-runtime /axis/opencv /
COPY --from=cv-sdk-runtime /axis/openblas /

CMD ["/usr/bin/capture_app"]