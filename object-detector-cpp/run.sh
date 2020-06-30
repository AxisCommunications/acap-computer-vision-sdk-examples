#!/bin/sh -e

if [ "${CAMERA_IP:-}" = "" ]; then
  if [ "${1:-}" = "" ]; then
    echo "Manually set CAMERA_IP or supply it as first argument" 1>&2
    exit 1
  else
    CAMERA_IP=$1
  fi
fi

docker-compose -H tcp://"$CAMERA_IP":2375 up
