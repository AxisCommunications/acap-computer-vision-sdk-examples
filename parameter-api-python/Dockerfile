# syntax=docker/dockerfile:1

ARG RUNTIME_VERSION=2.0.1
ARG REPO=axisecp
ARG IMAGE=acap-runtime
ARG UBUNTU_VERSION=24.04
ARG DEVICE_PLATFORM=linux/arm64/v8

FROM --platform=$DEVICE_PLATFORM ${REPO}/${IMAGE}:${RUNTIME_VERSION}-protofiles AS proto-image
FROM --platform=$DEVICE_PLATFORM ubuntu:${UBUNTU_VERSION} AS runtime-image

RUN DEBIAN_FRONTEND=noninteractive \
    apt-get update && apt-get install -y --no-install-recommends \
    python3 \
    python3-pip \
    python3-venv \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /opt/certificates
RUN openssl req -x509 -batch -subj '/CN=localhost' -days 365 -newkey rsa:4096 -nodes -out server.pem -keyout server.key

# Copy list of grpc dependencies
COPY --from=proto-image /build/requirements.txt .

# Create a virtual environment for installations using pip
RUN python3 -m venv /opt/venv
ENV PATH="/opt/venv/bin:$PATH"
RUN pip install --no-cache-dir -r requirements.txt

WORKDIR /opt/app

# Install prebuilt parameter proto files
COPY --from=proto-image /build/param/proto_utils .

# Copy the application script to the container
COPY app/* .

ENTRYPOINT  [ "python3", "parameter.py", "/opt/certificates/server.pem" ]
