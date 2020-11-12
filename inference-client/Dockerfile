# This Dockerfile creates a Docker image for inference-client
ARG REPO=axisecp

FROM ${REPO}/python-tfserving:1.0.0-armv7hf
LABEL version="1.0.0" \
      maintainer="rapid@axis.com"

WORKDIR /usr/lib/python3/site-packages/inference-client
COPY . .
CMD [ "-u", "detector.py" ]