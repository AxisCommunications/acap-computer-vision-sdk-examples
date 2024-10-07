# syntax=docker/dockerfile:1

ARG RUNTIME_VERSION=2.0.0
ARG REPO=axisecp
ARG IMAGE=acap-runtime
ARG UBUNTU_VERSION=22.04

FROM ${REPO}/${IMAGE}:${RUNTIME_VERSION}-protofiles AS proto-image
FROM arm64v8/ubuntu:${UBUNTU_VERSION} AS runtime-image

RUN DEBIAN_FRONTEND=noninteractive \
    apt-get update && apt-get install -y --no-install-recommends \
    python3 \
    python3-pip \
    ffmpeg \
    libsm6 \
    libxext6 \
    && rm -rf /var/lib/apt/lists/*

# Copy list of grpc dependencies
COPY --from=proto-image /build/requirements.txt .

RUN pip install --no-cache-dir -r requirements.txt \
    numpy \
    opencv-python

WORKDIR /opt/app

# Install prebuilt TensorFlow and VDO proto files
COPY --from=proto-image /build/tf/proto_utils .
COPY --from=proto-image /build/vdo/proto_utils .

# Copy the application script to the container
COPY app/* .

CMD ["python3", "detector.py"]
