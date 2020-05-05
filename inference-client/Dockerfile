# This Dockerfile creates a Docker image for inference-client,
# which is a testprogram for inference-server on camera
#
# Example build command:
#     docker build -t inference-client .
#     docker tag inference-client axisecp/inference-client:0.0.1
#     docker push axisecp/inference-client:0.0.1
#
# To clear inference-client on camera
#   $ docker -H tcp://172.25.75.141 system prune -f
#   $ docker -H tcp://172.25.75.141 image rm axisecp/inference-client:0.0.1
#
# To run on camera
#   $ docker -H tcp://172.25.75.141 run --rm axisecp/inference-client:0.0.1

FROM axisecp/python-tfserving:0.0.2
LABEL version="0.3" \
      maintainer="rapid@axis.com"

WORKDIR /usr/lib/python3/site-packages/inference-client
COPY . .
CMD [ "-u", "detector.py" ]