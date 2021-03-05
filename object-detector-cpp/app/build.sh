#!/bin/sh -e

if [ "${OBJECT_DETECTOR_CPP:-}" = "" ]; then
  if [ "${1:-}" = "" ]; then
    echo "Manually set OBJECT_DETECTOR_CPP or supply it an argument" 1>&2
    exit 1
  else
    OBJECT_DETECTOR_CPP=$1
  fi
fi

[ -n "$REPO" ] && repo="--build-arg REPO=$REPO"
[ -n "$ARCH" ] && arch="--build-arg ARCH=$ARCH"
[ -n "$VERSION" ] && version="--build-arg VERSION=$VERSION"
[ -n "$DOCKER_PROXY" ] && docker_proxy="--build-arg DOCKER_PROXY=$DOCKER_PROXY"

docker build . --tag $OBJECT_DETECTOR_CPP $repo $arch $version $docker_proxy
