# Should be used for getting API image
# Currently, only armv7hf is supported
ARG ARCH=armv7hf
ARG VERSION=4.0-pre3
ARG REPO=axisecp
ARG UBUNTU_VERSION=20.04

FROM $REPO/acap4-api:$VERSION-$ARCH-ubuntu$UBUNTU_VERSION as api

ARG DOCKER_PROXY
ARG http_proxy=$DOCKER_PROXY
ARG https_proxy=$DOCKER_PROXY

# Use a sources.list that includes armhf repositories
COPY sources.list /etc/apt

ENV DEBIAN_FRONTEND=noninteractive
ENV ARCH=armhf
ENV ARCHDIR=arm-linux-gnueabihf

RUN apt-get update && apt-get install -y -f \
    g++-arm-linux-gnueabihf \
    make\
    libglib2.0-dev \
    libsystemd0

RUN dpkg --add-architecture armhf &&\
    apt-get update && apt-get install -y -f \
        libglib2.0-dev:armhf \
        libsystemd0:armhf \
        libgrpc++-dev:armhf \
        libprotobuf-dev:armhf \
        libc-ares-dev:armhf \
        libgoogle-perftools-dev:armhf \
        libssl-dev:armhf \
        libcrypto++-dev:armhf

COPY Makefile /build/
COPY src/ /build/src/
WORKDIR /build
RUN make

FROM arm32v7/ubuntu:$UBUNTU_VERSION
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

# dependencies for grpc
COPY --from=api /usr/lib/arm-linux-gnueabihf/libprotobuf* /usr/lib/
COPY --from=api /usr/lib/arm-linux-gnueabihf/libgrpc* /usr/lib/
COPY --from=api /usr/lib/arm-linux-gnueabihf/libgpr* /usr/lib/
COPY --from=api /usr/lib/arm-linux-gnueabihf/libcares* /usr/lib/
COPY --from=api /usr/lib/arm-linux-gnueabihf/libprofiler* /usr/lib/
COPY --from=api /usr/lib/arm-linux-gnueabihf/libz* /usr/lib/
COPY --from=api /usr/lib/arm-linux-gnueabihf/libssl* /usr/lib/
COPY --from=api /usr/lib/arm-linux-gnueabihf/libcrypto* /usr/lib/

COPY --from=api /build/objdetector /usr/bin/

ENTRYPOINT []
CMD ["/usr/bin/objdetector"]


