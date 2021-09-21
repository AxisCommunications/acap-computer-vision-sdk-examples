#!/bin/sh -e

if [ "${CAMERA_IP:-}" = "" ]; then
  if [ "${1:-}" = "" ]; then
    echo "Manually set CAMERA_IP or supply it as first argument" 1>&2
    exit 1
  else
    CAMERA_IP=$1
  fi
fi

if [ "${APP_NAME:-}" = "" ]; then
  if [ "${2:-}" = "" ]; then
    echo "Manually set APP_NAME or supply it as second argument" 1>&2
    exit 1
  else
    APP_NAME=$2
  fi
fi

docker save $APP_NAME -o app.tar
docker -H tcp://$CAMERA_IP load -i app.tar
