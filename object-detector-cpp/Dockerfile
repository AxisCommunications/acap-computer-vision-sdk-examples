ARG ARCH=armv7hf
ARG SDK_VERSION=1.0-alpha1
ARG REPO=axisecp
ARG RUNTIME_IMAGE=arm32v7/ubuntu:20.04

FROM $REPO/acap-computer-vision-sdk:$SDK_VERSION-$ARCH as cv-sdk-runtime
FROM $REPO/acap-computer-vision-sdk:$SDK_VERSION-$ARCH-devel as cv-sdk-devel

# Setup proxy configuration
ARG DOCKER_PROXY
ENV http_proxy=$DOCKER_PROXY
ENV https_proxy=$DOCKER_PROXY
ENV HTTP_PROXY=$DOCKER_PROXY
ENV HTTPS_PROXY=$DOCKER_PROXY

ENV DEBIAN_FRONTEND=noninteractive

# TARGET_NAME is defined as the platform arch string
# i.e., arm-linux-gnueabihf for the ARTPEC-7 platform
RUN apt-get update && apt-get install -y -f \
    libglib2.0-dev \
    libsystemd0

# Download the libs required by the app and put them in /target-root to
# separate them, which makes copying them to the runtime environment easier 
RUN mkdir -p /tmp/libs /target-root
RUN apt-get update && apt-get install --reinstall --download-only -o=dir::cache=/tmp/libs -y -f \
        libglib2.0-dev:$UBUNTU_ARCH \
        libsystemd0:$UBUNTU_ARCH \
        libgrpc++-dev:$UBUNTU_ARCH \
        libprotobuf-dev:$UBUNTU_ARCH \
        libc-ares-dev:$UBUNTU_ARCH \
        libgoogle-perftools-dev:$UBUNTU_ARCH \
        libssl-dev:$UBUNTU_ARCH \
        libcrypto++-dev:$UBUNTU_ARCH
RUN for f in /tmp/libs/archives/*.deb; do dpkg -x $f /target-root; done
RUN cp -r /target-root/lib/* /target-root/usr/lib/ && rm -rf /target-root/lib

COPY app/Makefile /build/
COPY app/src/ /build/src/
WORKDIR /build
RUN make


FROM ${RUNTIME_IMAGE}
# Copy the libs that were downloaded in the previous stage
COPY --from=cv-sdk-devel /target-root /

# Copy the compiled object detection application
COPY --from=cv-sdk-devel /build/objdetector /usr/bin/

# Copy the precompiled opencv libs
COPY --from=cv-sdk-runtime /axis/opencv /

# Copy the precompiled openblas libs
COPY --from=cv-sdk-runtime /axis/openblas /

CMD ["/usr/bin/objdetector"]


