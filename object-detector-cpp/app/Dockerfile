# Should be used for getting API image
# Currently, only armv7hf is supported
ARG ARCH=armv7hf
ARG VERSION=4.0-pre2
ARG REPO=axisecp
ARG UBUNTU_VERSION=19.10

FROM ${REPO}/acap4-api:${VERSION}-${ARCH}-ubuntu${UBUNTU_VERSION} as api
FROM ${REPO}/acap4-toolchain:${VERSION}-${ARCH}-ubuntu${UBUNTU_VERSION} as toolchain

ARG DOCKER_PROXY
ARG http_proxy=${DOCKER_PROXY}
ARG https_proxy=${DOCKER_PROXY}

RUN apt-get update && apt-get install -y \
        libgrpc++-dev:armhf \
        libprotobuf-dev:armhf \
        libc-ares-dev:armhf \
        libgoogle-perftools-dev:armhf \
        libssl-dev:armhf \
        libcrypto++-dev:armhf

COPY --from=api /axis/opencv /axis/opencv/
COPY --from=api /axis/tf-proto /axis/tf-proto/

COPY Makefile /build/
COPY src/ /build/src/
WORKDIR /build
RUN make

FROM arm32v7/ubuntu:${UBUNTU_VERSION}

COPY --from=api /axis/opencv/lib/* /usr/lib/

COPY --from=toolchain /usr/lib/arm-linux-gnueabihf/libprotobuf* /usr/lib/
COPY --from=toolchain /usr/lib/arm-linux-gnueabihf/libgrpc* /usr/lib/
COPY --from=toolchain /usr/lib/arm-linux-gnueabihf/libgpr* /usr/lib/
COPY --from=toolchain /usr/lib/arm-linux-gnueabihf/libcares* /usr/lib/
COPY --from=toolchain /usr/lib/arm-linux-gnueabihf/libprofiler* /usr/lib/
COPY --from=toolchain /usr/lib/arm-linux-gnueabihf/libz* /usr/lib/
COPY --from=toolchain /usr/lib/arm-linux-gnueabihf/libssl* /usr/lib/
COPY --from=toolchain /usr/lib/arm-linux-gnueabihf/libcrypto* /usr/lib/

COPY --from=toolchain /build/objdetector /usr/bin/

ENTRYPOINT []
CMD ["/usr/bin/objdetector"]
