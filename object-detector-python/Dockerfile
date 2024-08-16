# syntax=docker/dockerfile:1

ARG REPO=axisecp
ARG SDK_VERSION=1.15
ARG UBUNTU_VERSION=22.04


FROM ${REPO}/acap-computer-vision-sdk:${SDK_VERSION}-aarch64 AS cv-sdk
FROM arm64v8/ubuntu:${UBUNTU_VERSION}

COPY --from=cv-sdk /axis/python /
COPY --from=cv-sdk /axis/python-numpy /
COPY --from=cv-sdk /axis/python-tfserving /
COPY --from=cv-sdk /axis/python-vdoproto /
COPY --from=cv-sdk /axis/opencv /
COPY --from=cv-sdk /axis/openblas /

WORKDIR /app
COPY app/* /app/
CMD ["python3", "detector.py"]
