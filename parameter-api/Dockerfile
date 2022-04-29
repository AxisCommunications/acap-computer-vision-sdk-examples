ARG ARCH=armv7hf
ARG SDK_VERSION=1.2
ARG REPO=axisecp
ARG UBUNTU_IMAGE_VER=20.04

FROM arm32v7/ubuntu:${UBUNTU_IMAGE_VER} as runtime-image-armv7hf
FROM arm64v8/ubuntu:${UBUNTU_IMAGE_VER} as runtime-image-aarch64
FROM $REPO/acap-computer-vision-sdk:$SDK_VERSION-$ARCH as cv-sdk-runtime
FROM $REPO/acap-computer-vision-sdk:$SDK_VERSION-$ARCH-devel as cv-sdk-devel

ENV DEBIAN_FRONTEND=noninteractive

# TARGET_TOOLCHAIN is defined as the platform arch string
# i.e., arm-linux-gnueabihf for the ARTPEC-7 platform
RUN apt-get update && apt-get install -y -f \
    libglib2.0-dev \
    libsystemd0 \
    protobuf-compiler-grpc && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

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
        libprotobuf17:$UBUNTU_ARCH \
        libc-ares2:$UBUNTU_ARCH
RUN for f in /tmp/runtime/archives/*.deb; do dpkg -x $f /target-root; done
RUN cp -r /target-root/lib/* /target-root/usr/lib/ && rm -rf /target-root/lib

# Generate TSL/SSL test certificate
RUN openssl req -x509 -batch -subj '/CN=localhost' -days 10000 -newkey rsa:4096 -nodes -out server.pem -keyout server.key

COPY app/ build/
WORKDIR /build
RUN make

FROM runtime-image-${ARCH}
# Copy the libraries needed for our runtime
COPY --from=cv-sdk-devel /target-root /

# Copy the compiled Parameter application
COPY --from=cv-sdk-devel /build/parameter /usr/bin/
COPY --from=cv-sdk-devel server.key server.pem models/

CMD ["usr/bin/parameter"]
