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

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
        crossbuild-essential-armhf \
        make \
        pkg-config

COPY Makefile /build/
COPY src/ /build/src/
WORKDIR /build
RUN make

FROM arm32v7/ubuntu:${UBUNTU_VERSION}

COPY --from=api /axis/opencv/lib/libopencv_core.so* /usr/lib/
COPY --from=api /axis/opencv/lib/libopencv_imgcodecs.so* /usr/lib/
COPY --from=api /axis/opencv/lib/libopencv_imgproc.so* /usr/lib/
COPY --from=api /build/capture_app /usr/bin/

WORKDIR /usr/bin

ENTRYPOINT [""]
CMD ["/usr/bin/capture_app"]

