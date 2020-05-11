# Local variables
DOCKER_REGISTRY="my-docker-registry.com"
APP_REPO="acap4-object-detector-cpp"
APP_VERSION="1.0.0"

# Exported varaibles used by scripts
export CAMERA_IP="192.168.0.90"
export APP_IMAGE=$DOCKER_REGISTRY/$APP_REPO:$APP_VERSION
