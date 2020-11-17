# This Dockerfile creates a Docker image for inference-client
ARG PYTHON_TFSERVING=axisecp/python-tfserving:1.1.0-armv7hf-ubuntu19.10

FROM ${PYTHON_TFSERVING}
LABEL maintainer="rapid@axis.com"

WORKDIR /usr/lib/python3/site-packages/inference-client
COPY . .
CMD [ "-u", "detector.py" ]