ARG ARCH=armv7hf
ARG REPO=axisecp
ARG SDK_VERSION=1.2.1
ARG UBUNTU_VERSION=20.04

FROM arm32v7/ubuntu:${UBUNTU_VERSION} as runtime-image-armv7hf
FROM arm64v8/ubuntu:${UBUNTU_VERSION} as runtime-image-aarch64

FROM ${REPO}/acap-computer-vision-sdk:${SDK_VERSION}-${ARCH} AS cv-sdk
FROM runtime-image-${ARCH}

COPY --from=cv-sdk /axis/python /
COPY --from=cv-sdk /axis/python-numpy /
COPY --from=cv-sdk /axis/python-tfserving /
COPY --from=cv-sdk /axis/opencv /
COPY --from=cv-sdk /axis/openblas /

RUN pip install Flask

WORKDIR /app
COPY app/ /app/
CMD ["python3", "detector_with_flask.py"]
