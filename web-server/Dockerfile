ARG ARCH=armv7hf
ARG UBUNTU_VERSION=20.04

FROM arm32v7/ubuntu:${UBUNTU_VERSION} as runtime-image-armv7hf
FROM arm64v8/ubuntu:${UBUNTU_VERSION} as runtime-image-aarch64

FROM runtime-image-${ARCH}

# Setup environment variables
ENV DEBIAN_FRONTEND=noninteractive
ENV BUILD_ROOT=/build-root

 ## Install dependencies
ARG ARCH
RUN apt-get update && apt-get install -y -f \
        git \
        make\
        cmake\
        pkg-config \
        libglib2.0-dev \
        libsystemd0

RUN if [ ${ARCH} = armv7hf ]; then \
        apt-get install -y -f \
        g++-arm-linux-gnueabihf &&\
        dpkg --add-architecture armhf &&\
        apt-get update && apt-get install -y  \
        libglib2.0-dev:armhf \
        libc-dev:armhf \
        libsystemd0:armhf; \
    elif [ ${ARCH} = aarch64 ]; then \
        apt-get install -y -f \
        g++-aarch64-linux-gnu &&\
        dpkg --add-architecture arm64 &&\
        apt-get update && apt-get install -y  \
        libglib2.0-dev:arm64 \
        libc-dev:arm64 \
        libsystemd0:arm64; \
    else \
        printf "Error: '%s' is not a valid value for the ARCH variable\n", ${ARCH}; \
        exit 1; \
    fi
WORKDIR /opt
COPY monkey.patch .

RUN git clone -b v1.5.6 https://github.com/monkey/monkey
WORKDIR /opt/monkey
RUN git apply ../monkey.patch &&\
    ./configure \
    --enable-shared \
    --malloc-libc \
    --prefix=/usr/local \
    --bindir=/usr/local/bin \
    --libdir=/usr/local/lib \
    --sysconfdir=/usr/local/packages/monkey/html \
    --datadir=/usr/local/packages/monkey/html \
    --mandir=/usr/local/man \
    --logdir=/tmp \
    --plugdir=/usr/local/packages/monkey/lib \
    --pidfile=/tmp/monkey.pid \
    --incdir=/usr/local/include/monkey \
    --systemddir=/usr/lib/systemd/system &&\
    make &&\
    make install

WORKDIR /opt/monkey/examples
RUN cp /usr/local/bin/monkey .

# Build examples
RUN sed -i 's/LDFLAGS/LDLIBS/g' Makefile &&\
    make &&\
    cp hello list quiz /usr/local/bin/

ENV LD_LIBRARY_PATH=/usr/local/lib

# Prepare data for examples
COPY . /tmp/
# Start Monkey server
CMD monkey
