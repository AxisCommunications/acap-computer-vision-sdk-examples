ARG ARCH=armv7hf
ARG SDK_VERSION=1.3
ARG REPO=axisecp

FROM arm32v7/ubuntu:20.04 as runtime-image-armv7hf
FROM arm64v8/ubuntu:20.04 as runtime-image-aarch64

# Specify which ACAP Computer Vision SDK version to use
FROM $REPO/acap-computer-vision-sdk:$SDK_VERSION-$ARCH AS cv-sdk

RUN apt-get update && apt-get install -y --no-install-recommends \
    ca-certificates \
    git \
    libprotobuf-dev \
    python3 \
    python3-pip \
    python3-grpc-tools \
    python3-protobuf \
    protobuf-compiler \
    protobuf-compiler-grpc \
    python3-grpcio

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

