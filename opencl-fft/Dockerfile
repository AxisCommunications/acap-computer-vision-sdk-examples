ARG ARCH=armv7hf
ARG VERSION=4.0-pre3
ARG REPO=axisecp
ARG UBUNTU_VERSION=19.10

# OpenCL libs are linked in from the host (AxOS) so we can use
# the OpenCL libs from acap3 when building.
FROM ${REPO}/acap-api:3.2-${ARCH}-ubuntu${UBUNTU_VERSION} as api3

# Use an ACAP4 image for compiling the OpenCL application
FROM ${REPO}/acap4-toolchain:${VERSION}-${ARCH}-ubuntu${UBUNTU_VERSION} as toolchain

# Setup proxy settings
ARG DOCKER_PROXY
ARG http_proxy=${DOCKER_PROXY}
ARG https_proxy=${DOCKER_PROXY}

ENV API3_ROOT=/opt/axis/sdk/temp/sysroots/cortexa9hf-neon-poky-linux-gnueabi/
ENV ARMHF_CPP="arm-linux-gnueabihf-g++ -mthumb -mfpu=neon -mfloat-abi=hard \
               -mcpu=cortex-a9"

# Add a sources.list that includes armhf repositories
COPY sources.list /etc/apt

# Get crosscompilation tool
RUN dpkg --add-architecture armhf &&\
    apt-get update && apt-get install -y -f \
    g++-arm-linux-gnueabihf

# Get the OpenCL application files
COPY app/* /app/src/

RUN mkdir -p /app/build && \
    mkdir -p /usr/include/CL

# Get the OpenCL headers and libraries needed for compilation
COPY --from=api3 $API3_ROOT/usr/include/CL/ /usr/include/CL/
COPY --from=api3 $API3_ROOT/usr/lib/libOpenCL.so /usr/lib/
COPY --from=api3 $API3_ROOT/usr/lib/libVivanteOpenCL.so /usr/lib/

# Compile the OpenCL application for our target platform
RUN $ARMHF_CPP \
-o /app/build/cl_test /app/src/cl_test.cpp /app/src/clutil.cpp \
-lOpenCL

# Define the target image
FROM arm32v7/ubuntu:19.10

WORKDIR /app

# Copy the compiled application files from the builder container
COPY --from=toolchain /app/build/cl_test .
COPY --from=toolchain /app/src/fft.cl .

ENTRYPOINT ["./cl_test"]
CMD ["1"]
