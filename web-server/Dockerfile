# syntax=docker/dockerfile:1

ARG UBUNTU_VERSION=22.04

FROM arm64v8/ubuntu:${UBUNTU_VERSION} as runtime-image

# Setup environment variables
ENV DEBIAN_FRONTEND=noninteractive
ENV BUILD_ROOT=/build-root

 ## Install dependencies
RUN <<EOF
apt-get update
apt-get install -y -f git make cmake pkg-config libglib2.0-dev libsystemd0
EOF

RUN <<EOF
    apt-get install -y -f g++-aarch64-linux-gnu
    dpkg --add-architecture arm64
    apt-get update
    apt-get install -y libglib2.0-dev:arm64 libc-dev:arm64 libsystemd0:arm64
EOF
WORKDIR /opt
COPY monkey.patch .

RUN git clone -b v1.5.6 https://github.com/monkey/monkey
WORKDIR /opt/monkey
RUN <<EOF
git apply ../monkey.patch
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
    --systemddir=/usr/lib/systemd/system
make
make install
EOF

WORKDIR /opt/monkey/examples
RUN cp /usr/local/bin/monkey .

# Build examples
RUN <<EOF
sed -i 's/LDFLAGS/LDLIBS/g' Makefile
make
cp hello list quiz /usr/local/bin/
EOF

ENV LD_LIBRARY_PATH=/usr/local/lib

# Prepare data for examples
COPY . /tmp/
# Start Monkey server
CMD monkey
