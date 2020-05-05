# Should be used for getting API image
# Currently, only armv7hf is supported
ARG ARCH=armv7hf
ARG VERSION_API=4.0-pre2

FROM axisecp/acap4-api:${VERSION_API}-${ARCH} as api

ARG DOCKER_PROXY
ARG http_proxy=${DOCKER_PROXY}
ARG https_proxy=${DOCKER_PROXY}

RUN dpkg --add-architecture armhf

# Use a sources.list that includes armhf repositories
COPY sources.list /etc/apt

RUN apt-get update && apt-get install -y \
        libgrpc++-dev:armhf \
        libprotobuf-dev:armhf \
        libc-ares-dev:armhf \
        libgoogle-perftools-dev:armhf \
        libssl-dev:armhf \
        libcrypto++-dev:armhf

RUN apt-get update && apt-get install -y --no-install-recommends \
        crossbuild-essential-armhf \
        make \
        pkg-config

COPY Makefile /build/
COPY src/ /build/src/
WORKDIR /build
RUN make

FROM arm32v7/ubuntu:19.10

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



