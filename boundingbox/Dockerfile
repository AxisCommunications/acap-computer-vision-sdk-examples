# Specify the architecture at build time: aarch64
# Should be used for getting API and toolchain images
# Currently, only aarch64 is supported
ARG ARCH=aarch64
ARG VERSION=3.2
ARG UBUNTU_VERSION=19.10
ARG REPO=axisecp

FROM ${REPO}/acap-sdk:${VERSION}-${ARCH}-ubuntu${UBUNTU_VERSION}

# Building the ACAP application
COPY ./app /opt/app/
WORKDIR /opt/app
RUN . /opt/axis/acapsdk/environment-setup* && create-package.sh
