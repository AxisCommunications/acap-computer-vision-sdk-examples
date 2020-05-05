#!/bin/sh -eu

if [ "${CAMERA_IP:-}" == "" ]; then
  if [ "${1:-}" == "" ]; then
    echo "Manually set CAMERA_IP or supply it as first argument" 1>&2
    exit -1
  else
    CAMERA_IP=$1
  fi
fi

if [ "${APP_IMAGE:-}" == "" ]; then
  if [ "${2:-}" == "" ]; then
    echo "Manually set APP_IMAGE or supply it as second argument" 1>&2
    exit -1
  else
    APP_IMAGE=$2
  fi
fi

cd app && ./build.sh $APP_IMAGE && ./upload.sh $CAMERA_IP $APP_IMAGE && cd .. &&
  ./run.sh $CAMERA_IP
