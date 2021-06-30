ARG UBUNTU_VERSION=20.04
ARG http_proxy
ARG https_proxy

# A base image for crosscompilation
FROM ubuntu:$UBUNTU_VERSION AS build-base

# To support DOCKER_BUILDKIT=0, base ARGs are converted to ENVs to allow propagation
ENV http_proxy=$http_proxy
ENV https_proxy=$https_proxy

# Setup environment variables
ENV DEBIAN_FRONTEND=noninteractive
ENV BUILD_ROOT=/build-root
ENV CC="arm-linux-gnueabihf-gcc -mthumb -mfpu=neon -mfloat-abi=hard -mcpu=cortex-a9"

# Add source for target arch
RUN echo \
"deb [arch=amd64] http://us.archive.ubuntu.com/ubuntu/ focal main restricted universe multiverse\n\
deb [arch=amd64] http://us.archive.ubuntu.com/ubuntu/ focal-updates main restricted universe multiverse\n\
deb [arch=amd64] http://us.archive.ubuntu.com/ubuntu/ focal-backports main restricted universe multiverse\n\
deb [arch=amd64] http://security.ubuntu.com/ubuntu focal-security main restricted universe multiverse\n\
deb [arch=armhf,arm64] http://ports.ubuntu.com/ubuntu-ports/ focal main restricted universe multiverse\n\
deb [arch=armhf,arm64] http://ports.ubuntu.com/ubuntu-ports/ focal-updates main restricted universe multiverse\n\
deb [arch=armhf,arm64] http://ports.ubuntu.com/ubuntu-ports/ focal-backports main restricted universe multiverse\n\
deb [arch=armhf,arm64] http://ports.ubuntu.com/ubuntu-ports/ focal-security main restricted universe multiverse"\
 > /etc/apt/sources.list

 ## Install dependencies
RUN dpkg --add-architecture armhf &&\
    apt-get update && apt-get install -y -f \
    git \
    make \
    cmake \
    pkg-config \
    g++-arm-linux-gnueabihf \
    libc-dev:armhf

# Build server and library
WORKDIR $BUILD_ROOT
RUN git clone -b v1.5.6 https://github.com/monkey/monkey &&\
    cd monkey &&\
    mkdir -p build &&\
    ./configure \
        --enable-shared \
        --malloc-libc \
        --prefix=/usr/local \
        --plugdir=/usr/local/lib \
        --pidfile=/usr/local/logs/monkey.pid \
        --incdir=/usr/local/include/monkey \
        --systemddir=/usr/lib/systemd/system \
        --default-port=80 &&\
    make &&\
    make install

# Build examples
ARG PKG_CONFIG_PATH=/usr/local/lib/pkgconfig
RUN cd monkey/examples &&\
    sed -i 's/LDFLAGS/LDLIBS/g' Makefile &&\
    make &&\
    cp hello list quiz /usr/local/bin/

# Create target image
FROM arm32v7/ubuntu:$UBUNTU_VERSION
ENV LD_LIBRARY_PATH=/usr/local/lib
COPY --from=build-base /usr/local /usr/local

# Prepare data for examples
COPY . /tmp/

# Start Monkey server
CMD monkey
