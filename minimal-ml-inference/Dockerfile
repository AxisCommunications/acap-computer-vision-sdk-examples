ARG ARCH=armv7hf
ARG SDK_VERSION=1.2.1
ARG REPO=axisecp

FROM arm32v7/ubuntu:20.04 as runtime-image-armv7hf
FROM arm64v8/ubuntu:20.04 as runtime-image-aarch64

# Specify which ACAP Computer Vision SDK to use
FROM $REPO/acap-computer-vision-sdk:$SDK_VERSION-$ARCH AS cv-sdk

# Define the runtime image
FROM runtime-image-${ARCH}

# Get packages from the CV SDK
COPY --from=cv-sdk /axis/python /
COPY --from=cv-sdk /axis/python-tfserving /
COPY --from=cv-sdk /axis/python-numpy /
COPY --from=cv-sdk /axis/opencv /
COPY --from=cv-sdk /axis/openblas /

# Copy the application script to the container
WORKDIR /app
COPY simple_inference.py .
ENTRYPOINT ["python3", "simple_inference.py"]
