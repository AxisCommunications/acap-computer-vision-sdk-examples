# Specify the architecture at build time: mipsis32r2el/armv7hf/aarch64
# Should be used for getting API image
# Currently, only armv7hf is supported
ARG ARCH=armv7hf
ARG REPO=axisecp
ARG SDK_VERSION=1.2.1
ARG UBUNTU_VERSION=20.04

FROM arm32v7/ubuntu:${UBUNTU_VERSION} as runtime-image-armv7hf
FROM arm64v8/ubuntu:${UBUNTU_VERSION} as runtime-image-aarch64

FROM ${REPO}/acap-computer-vision-sdk:${SDK_VERSION}-${ARCH}-runtime AS cv-sdk-runtime
FROM ${REPO}/acap-computer-vision-sdk:${SDK_VERSION}-${ARCH}-devel AS cv-sdk-devel

# Setup proxy configuration
ARG HTTP_PROXY
ENV http_proxy=${HTTP_PROXY}
ENV https_proxy=${HTTP_PROXY}

ENV DEBIAN_FRONTEND=noninteractive

## Install dependencies
ARG ARCH
RUN apt-get update && apt-get install -y -f \
        make\
        pkg-config \
        libglib2.0-dev \
        libsystemd0

RUN if [ ${ARCH} = armv7hf ]; then \
        apt-get install -y -f \
        g++-arm-linux-gnueabihf &&\
        dpkg --add-architecture armhf &&\
        apt-get update && apt-get install -y  \
        libglib2.0-dev:armhf \
        libsystemd0:armhf; \
    elif [ ${ARCH} = aarch64 ]; then \
        apt-get install -y -f \
        g++-aarch64-linux-gnu &&\
        dpkg --add-architecture arm64 &&\
        apt-get update && apt-get install -y  \
        libglib2.0-dev:arm64 \
        libsystemd0:arm64; \
    else \
        printf "Error: '%s' is not a valid value for the ARCH variable\n", ${ARCH}; \
        exit 1; \
    fi

COPY app/Makefile /build/
COPY app/src/ /build/src/
WORKDIR /build

RUN if [ ${ARCH} = armv7hf ]; then \
        make CXX=arm-linux-gnueabihf-g++ CC=arm-linux-gnueabihf-gcc  STRIP=arm-linux-gnueabihf-strip;\
    elif [ ${ARCH} = aarch64 ]; then \
        make  CXX=/usr/bin/aarch64-linux-gnu-g++ CC=/usr/bin/aarch64-linux-gnu-gcc STRIP=aarch64-linux-gnu-strip;\
    else \
        printf "Error: '%s' is not a valid value for the ARCH variable\n", ${ARCH}; \
        exit 1; \
    fi
    

FROM runtime-image-${ARCH}
COPY --from=cv-sdk-devel /build/capture_app /usr/bin/
COPY --from=cv-sdk-runtime /axis/opencv /
COPY --from=cv-sdk-runtime /axis/openblas /

CMD ["/usr/bin/capture_app"]
