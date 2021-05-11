#!/bin/sh -e

set -x

if [ "${APP_NAME:-}" = "" ]; then
  if [ "${1:-}" = "" ]; then

    echo "Manually set APP_NAME or supply it an argument" 1>&2
    exit 1
  else
    APP_NAME=$1
  fi
fi

[ -n "$REPO" ] && repo="--build-arg REPO=$REPO"
[ -n "$ARCH" ] && arch="--build-arg ARCH=$ARCH"
[ -n "$VERSION" ] && version="--build-arg VERSION=$VERSION"
[ -n "$DOCKER_PROXY" ] && docker_proxy="--build-arg DOCKER_PROXY=$DOCKER_PROXY"

docker build . --tag $APP_NAME $repo $arch $version $docker_proxy
