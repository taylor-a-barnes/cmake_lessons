#!/bin/sh

IMAGE=$(cat .docker/image_name)
PORT="${1:-0}"

if ! docker image inspect "$IMAGE" >/dev/null 2>&1; then
    echo "Image not found locally. Pulling $IMAGE..."
    if ! docker pull "$IMAGE"; then
        echo "Failed to pull image $IMAGE." >&2
        exit 1
    fi
    echo ""
    echo ""
    echo ""
fi

# Copy the run script from the image
CID=$(docker create $IMAGE)
docker cp $CID:/interface.sh .interface.sh > /dev/null
docker rm -v $CID > /dev/null

# Run the image's interface script
if [ "$PORT" -eq 0 ]; then
    bash .interface.sh $IMAGE
else
    bash .interface.sh $IMAGE $PORT
fi