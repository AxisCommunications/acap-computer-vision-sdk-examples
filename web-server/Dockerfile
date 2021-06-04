#-- ACAP SDK configuration --#
ARG REPO=axisecp
ARG ACAP_SDK_IMAGE=acap-native-sdk
ARG ACAP_SDK_VERSION=1.0_alpha1
ARG AXIS_ARCH=armv7hf
ARG UBUNTU_VERSION=20.04
ARG ACAP_SDK_TAG=$ACAP_SDK_VERSION-$AXIS_ARCH-ubuntu$UBUNTU_VERSION
ARG BUILD_ROOT=/build-root

# The ACAP Native SDK is used to get certain libs and headers
FROM $REPO/$ACAP_SDK_IMAGE:$ACAP_SDK_TAG AS acap-native-sdk
# libs: /opt/axis/acapsdk/sysroots/$AXIS_ARCH/usr/lib/
# headers: /opt/axis/acapsdk/sysroots/$AXIS_ARCH/usr/include/

# A base image with common tools for crosscompilation
FROM ubuntu:$UBUNTU_VERSION AS build-base

# Setup environment variables
ENV DEBIAN_FRONTEND=noninteractive
ARG BUILD_ROOT
ARG http_proxy
ARG https_proxy

## Build environment
ENV CC="arm-linux-gnueabihf-gcc -mthumb -mfpu=neon -mfloat-abi=hard -mcpu=cortex-a9"
ENV CXX="arm-linux-gnueabihf-g++ -mthumb -mfpu=neon -mfloat-abi=hard -mcpu=cortex-a9"
ENV CPP="arm-linux-gnueabihf-gcc -E -mthumb -mfpu=neon -mfloat-abi=hard -mcpu=cortex-a9"
ENV LIB=/usr/lib/arm-linux-gnueabihf
ENV PKG_CONFIG_LIBDIR=$PKG_CONFIG_LIBDIR:$LIB/pkgconfig

# To support DOCKER_BUILDKIT=0, base ARGs are converted to ENVs to allow propagation
ENV BUILD_ROOT=$BUILD_ROOT
ENV http_proxy=$http_proxy
ENV https_proxy=$https_proxy

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
    g++-arm-linux-gnueabihf \
    libc-dev:armhf

WORKDIR $BUILD_ROOT
RUN git clone -b v1.6.9 https://github.com/monkey/monkey &&\
    cd monkey &&\
    mkdir -p build &&\
    ./configure\
        --local\
        --static-lib-mode\
        --static-plugins=auth,fastcgi,tls,dirlisting,cheetah,liana,logger\
        --disable-plugins=cgi,cheetah,mandril &&\
    cd build &&\
    make

FROM arm32v7/ubuntu:$UBUNTU_VERSION as release
ARG BUILD_ROOT
COPY ./etc /etc
COPY ./var /var
COPY --from=build-base $BUILD_ROOT/monkey/build/monkey /usr/bin
CMD /usr/bin/monkey -c /etc/monkey -p 80
