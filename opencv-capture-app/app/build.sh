#!/bin/sh -e

if [ "${APP_IMAGE:-}" = "" ]; then
  if [ "${1:-}" = "" ]; then
    echo "Manually set APP_IMAGE or supply it an argument" 1>&2
    exit -1
  else
    APP_IMAGE=$1
  fi
fi

[ -n "$ARCH" ] && arch="--build-arg ARCH=$ARCH"
[ -n "$VERSION" ] && version="--build-arg VERSION=$VERSION"
[ -n "$DOCKER_PROXY" ] && docker_proxy="--build-arg DOCKER_PROXY=$DOCKER_PROXY"

docker build . --tag "$APP_IMAGE" "$arch" "$version" "$docker_proxy"
