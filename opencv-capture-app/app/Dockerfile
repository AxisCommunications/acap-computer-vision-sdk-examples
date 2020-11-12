# Specify the architecture at build time: mipsis32r2el/armv7hf/aarch64
# Should be used for getting API image
# Currently, only armv7hf is supported
ARG ARCH=armv7hf

ARG VERSION=4.0-pre3
ARG REPO=axisecp
ARG UBUNTU_VERSION=19.10

FROM ${REPO}/acap4-api:${VERSION}-${ARCH}-ubuntu${UBUNTU_VERSION} as api
FROM ${REPO}/acap4-toolchain:${VERSION}-${ARCH}-ubuntu${UBUNTU_VERSION} as toolchain


ARG DOCKER_PROXY
ARG http_proxy=${DOCKER_PROXY}
ARG https_proxy=${DOCKER_PROXY}

RUN apt-get update && apt-get install -y --no-install-recommends \
        crossbuild-essential-armhf \
        make \
        pkg-config

COPY --from=api /axis/opencv /axis/opencv/

COPY Makefile /build/
COPY src/ /build/src/
WORKDIR /build
RUN make

FROM arm32v7/ubuntu:19.10

COPY --from=toolchain /axis/opencv/lib/libopencv_core.so* /usr/lib/
COPY --from=toolchain /axis/opencv/lib/libopencv_imgcodecs.so* /usr/lib/
COPY --from=toolchain /axis/opencv/lib/libopencv_imgproc.so* /usr/lib/

COPY --from=toolchain /build/capture_app /usr/bin/

ENTRYPOINT [""]
CMD ["/usr/bin/capture_app"]


