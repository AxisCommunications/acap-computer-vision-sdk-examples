ARG ARCH=armv7hf
ARG REPO=axisecp
ARG SDK_VERSION=1.2.1
ARG UBUNTU_VERSION=20.04

FROM arm32v7/ubuntu:${UBUNTU_VERSION} as runtime-image-armv7hf
FROM arm64v8/ubuntu:${UBUNTU_VERSION} as runtime-image-aarch64

FROM ${REPO}/acap-computer-vision-sdk:${SDK_VERSION}-${ARCH} as cv-sdk-runtime
FROM ${REPO}/acap-computer-vision-sdk:${SDK_VERSION}-${ARCH}-devel as cv-sdk-devel

# Setup proxy configuration
ARG HTTP_PROXY
ENV http_proxy=$(HTTP_PROXY)
ENV https_proxy=${HTTP_PROXY}

ENV DEBIAN_FRONTEND=noninteractive

# TARGET_TOOLCHAIN is defined as the platform arch string
# i.e., arm-linux-gnueabihf for the ARTPEC-7 platform
RUN apt-get update && apt-get install -y -f \
    libglib2.0-dev \
    libsystemd0

RUN mkdir -p /tmp/devel /tmp/runtime /build-root /target-root

# Download the target libs/headers required for compilation
RUN apt-get update && apt-get install --reinstall --download-only -o=dir::cache=/tmp/devel -y -f \
        libglib2.0-dev:$UBUNTU_ARCH \
        libsystemd0:$UBUNTU_ARCH \
        libgrpc++-dev:$UBUNTU_ARCH \
        libprotobuf-dev:$UBUNTU_ARCH \
        libc-ares-dev:$UBUNTU_ARCH \
        libgoogle-perftools-dev:$UBUNTU_ARCH \
        libssl-dev:$UBUNTU_ARCH \
        libcrypto++-dev:$UBUNTU_ARCH \
        libgcrypt20:$UBUNTU_ARCH
RUN for f in /tmp/devel/archives/*.deb; do dpkg -x $f /build-root; done
RUN cp -r /build-root/lib/* /build-root/usr/lib/ && rm -rf /build-root/lib


# Separate the target libs required during runtime
RUN apt-get update && apt-get install --reinstall --download-only -o=dir::cache=/tmp/runtime -y -f \
        libgrpc++:$UBUNTU_ARCH \
        '^libprotobuf([0-9]{1,2})$':${UBUNTU_ARCH} \
        libc-ares2:$UBUNTU_ARCH
RUN for f in /tmp/runtime/archives/*.deb; do dpkg -x $f /target-root; done
RUN cp -r /target-root/lib/* /target-root/usr/lib/ && rm -rf /target-root/lib


COPY app/Makefile /build/
COPY app/src/ /build/src/
WORKDIR /build
RUN make


FROM runtime-image-${ARCH}
# Copy the libraries needed for our runtime
COPY --from=cv-sdk-devel /target-root /

# Copy the compiled object detection application
COPY --from=cv-sdk-devel /build/objdetector /usr/bin/

# Copy the precompiled opencv libs
COPY --from=cv-sdk-runtime /axis/opencv /

# Copy the precompiled openblas libs
COPY --from=cv-sdk-runtime /axis/openblas /

CMD ["/usr/bin/objdetector"]
