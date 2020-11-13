# This Dockerfile creates a Docker image for inference-client
ARG PYTHON_TFSERVING

FROM ${PYTHON_TFSERVING}
LABEL maintainer="rapid@axis.com"

WORKDIR /usr/lib/python3/site-packages/inference-client
COPY . .
CMD [ "-u", "detector.py" ]