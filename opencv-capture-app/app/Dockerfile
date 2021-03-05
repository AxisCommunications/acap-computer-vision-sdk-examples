# Specify the architecture at build time: mipsis32r2el/armv7hf/aarch64
# Should be used for getting API image
# Currently, only armv7hf is supported
ARG ARCH=armv7hf
ARG VERSION=4.0-pre3
ARG REPO=axisecp
ARG UBUNTU_VERSION=20.04

FROM ${REPO}/acap4-api:${VERSION}-${ARCH}-ubuntu${UBUNTU_VERSION} as api

ARG DOCKER_PROXY
ARG http_proxy=${DOCKER_PROXY}
ARG https_proxy=${DOCKER_PROXY}

COPY sources.list /etc/apt
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

COPY Makefile /build/
COPY src/ /build/src/
WORKDIR /build
RUN make

FROM arm32v7/ubuntu:${UBUNTU_VERSION}

COPY --from=api /axis/opencv/lib/libopencv_core.so.4.?  /usr/lib/
COPY --from=api /axis/opencv/lib/libopencv_imgproc.so.4.?  /usr/lib/
COPY --from=api /axis/opencv/lib/libopencv_imgcodecs.so.4.?  /usr/lib/
# libcapaxis only exist for opencv 4.5.1 or later
COPY --from=api /axis/opencv/lib/libopencv_videoio.so.4.?  /axis/opencv/lib/libcapaxis* /usr/lib/

# dependencies for opencv
COPY --from=api /axis/device-api/lib/libvdostream.so.1 /usr/lib/
COPY --from=api /axis/device-api/lib/libfido.so.1 /usr/lib/
COPY --from=api /usr/lib/arm-linux-gnueabihf/libgobject-2.0* /usr/lib/
COPY --from=api /usr/lib/arm-linux-gnueabihf/libglib-2.0* /usr/lib/
COPY --from=api /usr/lib/arm-linux-gnueabihf/libgio-2.0* /usr/lib/
COPY --from=api /usr/lib/arm-linux-gnueabihf/libpcre* /usr/lib/
COPY --from=api /usr/lib/arm-linux-gnueabihf/libgmodule-2.0* /usr/lib/

COPY --from=api /build/capture_app /usr/bin/

WORKDIR /usr/bin

ENTRYPOINT [""]
CMD ["/usr/bin/capture_app"]
