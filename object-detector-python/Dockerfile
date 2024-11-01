# syntax=docker/dockerfile:1

ARG RUNTIME_VERSION=2.0.0
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
    ffmpeg \
    libsm6 \
    libxext6 \
    && rm -rf /var/lib/apt/lists/*

# Copy list of grpc dependencies
COPY --from=proto-image /build/requirements.txt .

# Create a virtual environment for installations using pip
RUN python3 -m venv /opt/venv
ENV PATH="/opt/venv/bin:$PATH"
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
