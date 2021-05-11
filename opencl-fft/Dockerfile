ARG ARCH=armv7hf
ARG SDK_VERSION=1.0-alpha1
ARG UBUNTU_VERSION=20.04
ARG REPO=axisecp
ARG RUNTIME_IMAGE=arm32v7/ubuntu:${UBUNTU_VERSION}

# OpenCL libs are linked in from the host (AxOS) so we can use
# the OpenCL libs from the devel-image of the CV SDK when building.
FROM $REPO/acap-computer-vision-sdk:$SDK_VERSION-$ARCH-devel AS cv-sdk

# Setup proxy settings
ARG DOCKER_PROXY
ARG http_proxy=$DOCKER_PROXY
ARG https_proxy=$DOCKER_PROXY

# Get the OpenCL application files
COPY app/* /app/src/

RUN mkdir -p /app/build

# ARCH_CFLAGS and TARGET_NAME are defined in the CV SDK
# They corresponds to the platform's C compilation flags
# and the platform's name     
ENV CXX="$TARGET_NAME-g++ $ARCH_CFLAGS"

# Compile the OpenCL application for our target platform
RUN $CXX -L /axis/opencl/usr/lib -I /axis/opencl/usr/include \
-o /app/build/cl_test /app/src/cl_test.cpp /app/src/clutil.cpp \
-lGAL -lVSC -lGLESv2 -lEGL -lOpenCL


# Define the target image
FROM ${RUNTIME_IMAGE}
WORKDIR /app

# Copy the compiled application files from the builder container
COPY --from=cv-sdk /app/build/cl_test .
COPY --from=cv-sdk /app/src/fft.cl .

ENTRYPOINT ["./cl_test"]
CMD ["1"]
