# syntax=docker/dockerfile:1

ARG ARCH=armv7hf
ARG REPO=axisecp
ARG SDK_VERSION=1.12_rc1
ARG UBUNTU_VERSION=22.04

FROM arm32v7/ubuntu:${UBUNTU_VERSION} as runtime-image-armv7hf
FROM arm64v8/ubuntu:${UBUNTU_VERSION} as runtime-image-aarch64

# Specify which ACAP Computer Vision SDK version to use
FROM ${REPO}/acap-computer-vision-sdk:${SDK_VERSION}-${ARCH} AS cv-sdk

RUN <<EOF
apt-get update
apt-get install -y --no-install-recommends \
    ca-certificates \
    git \
    libprotobuf-dev \
    python3 \
    python3-pip \
    protobuf-compiler \
    protobuf-compiler-grpc
EOF

# grpcio-tools contains protoc, which allows to build the pb2.py files in the install-tf.sh script.
# Installing directly grpcio-tools result in an older version of protobuf. 
# So we install first the dependencies with the desired version and then grpcio-tool without dependencies.
RUN python3 -m pip install protobuf==4.21.1 six==1.16.0 grpcio==1.46.3 && \
    python3 -m pip install --no-dependencies grpcio-tools==1.47.0

# Generate TSL/SSL test certificate
RUN openssl req -x509 -batch -subj '/CN=localhost' -days 365 -newkey rsa:4096 -nodes -out server.pem -keyout server.key

COPY app/apis build/
WORKDIR /build
ARG PROTO_FILE=keyvaluestore.proto
RUN python3 -m grpc_tools.protoc -I . --python_out=. --grpc_python_out=. $PROTO_FILE

# Define the runtime image
FROM runtime-image-${ARCH}

# Copy packages and files from CV-SDK
COPY --from=cv-sdk /axis/python /
COPY --from=cv-sdk /axis/python-tfserving /
COPY --from=cv-sdk server.key server.pem certificates/

COPY --from=cv-sdk /build/*.py opt/build/
COPY /app/src/parameter.py opt/build/
WORKDIR /opt/build
ENTRYPOINT [ "python3", "parameter.py" ]
