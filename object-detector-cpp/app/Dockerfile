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

## Install dependencies
RUN apt-get update && apt-get install -y -f \
    g++-arm-linux-gnueabihf \
    make

RUN dpkg --add-architecture armhf &&\
    apt-get update && apt-get install -y -f \
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

COPY --from=api /axis/opencv/lib/* /usr/lib/
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
