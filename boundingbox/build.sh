#!/bin/bash
UBUNTU_VERSION=19.10

if [ $# -lt 1 ]; then
    echo "Argument should be: <APP_IMAGE>"
    exit 1
fi

docker build --tag "$1" .

rm -rf build/
docker cp $(docker create "$1":latest):/opt/app ./build
