# Specify the architecture at build time: mipsis32r2el/armv7hf/aarch64
# Should be used for getting API image
# Currently, only armv7hf is supported
ARG ARCH=armv7hf
ARG VERSION_API=4.0-pre2

FROM axisecp/acap4-api:${VERSION_API}-${ARCH} as api

ARG DOCKER_PROXY
ARG http_proxy=${DOCKER_PROXY}
ARG https_proxy=${DOCKER_PROXY}

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
COPY --from=api /build/capture_app /usr/bin/

ENTRYPOINT [""]
CMD ["/usr/bin/capture_app"]
