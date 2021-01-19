ARG ARCH=armv7hf
ARG ACAPSDK_VERSION=3.2
ARG UBUNTU_VERSION=20.04
ARG REPO=axisecp

FROM $REPO/acap-sdk:$ACAPSDK_VERSION-$ARCH-ubuntu$UBUNTU_VERSION as sdk

# Setup proxy settings
ARG DOCKER_PROXY
ARG http_proxy=$DOCKER_PROXY
ARG https_proxy=$DOCKER_PROXY

# Building the ACAP application
COPY ./app /opt/app/
WORKDIR /opt/app
RUN . /opt/axis/acapsdk/environment-setup* && make

FROM arm32v7/ubuntu:$UBUNTU_VERSION
ARG LIB=/opt/axis/acapsdk/sysroots/cortexa9hf-neon-poky-linux-gnueabi/usr/lib
COPY --from=sdk /opt/app/bbox_example /usr/bin/
COPY --from=sdk $LIB/libbbox* /usr/lib/
COPY --from=sdk $LIB/libvdostream* /usr/lib/
COPY --from=sdk $LIB/libcairo* /usr/lib/
COPY --from=sdk $LIB/libjansson* /usr/lib/
COPY --from=sdk $LIB/libgio-2.0* /usr/lib/
COPY --from=sdk $LIB/libgobject-2.0* /usr/lib/
COPY --from=sdk $LIB/libglib-2.0* /usr/lib/
COPY --from=sdk $LIB/libfido* /usr/lib/
COPY --from=sdk $LIB/libpixman-1* /usr/lib/
COPY --from=sdk $LIB/libfontconfig* /usr/lib/
COPY --from=sdk $LIB/libfreetype* /usr/lib/
COPY --from=sdk $LIB/libpng16* /usr/lib/
COPY --from=sdk $LIB/libgmodule-2.0* /usr/lib/
COPY --from=sdk $LIB/libffi* /usr/lib/
COPY --from=sdk $LIB/libpcre* /usr/lib/
COPY --from=sdk $LIB/libexpat* /usr/lib/

ENTRYPOINT []
CMD ["/usr/bin/bbox_example"]

# The line below is used for finding out what libraries are needed
# and the outcome is the list above of libraries to preserve.
#ENTRYPOINT [ "/usr/bin/ldd", "/usr/bin/bbox_example" ]
