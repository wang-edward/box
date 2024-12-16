#!/bin/bash

# Set variables
IMAGE_NAME="box"
CONTAINER_NAME="container"

# Ensure .asoundrc exists on the host
if [ ! -f ~/.asoundrc ]; then
    echo 'pcm.!default { type plug slave.pcm "hw:0,0" }' > ~/.asoundrc
fi

# Allow X11 connections
xhost +local:docker

# Check if the Docker image exists
if ! docker image inspect $IMAGE_NAME >/dev/null 2>&1; then
    echo "Docker image '$IMAGE_NAME' not found. Building the image..."
    docker build -t $IMAGE_NAME . --progress=plain
else
    echo "Docker image '$IMAGE_NAME' already exists. Skipping build."
fi

# Run the Docker container with X11 and ALSA support
docker run --rm -it \
    --name $CONTAINER_NAME \
    --device /dev/snd \
    -v /usr/share/alsa:/usr/share/alsa \
    -v /etc/asound.conf:/etc/asound.conf \
    -v ~/.asoundrc:/root/.asoundrc \
    -e DISPLAY=$DISPLAY \
    -v /tmp/.X11-unix:/tmp/.X11-unix \
    -e XDG_RUNTIME_DIR=/tmp/xdg_runtime_dir \
    --tmpfs /tmp/xdg_runtime_dir:rw,mode=0700 \
    $IMAGE_NAME  bash


# docker run --rm -it \
#     --name $CONTAINER_NAME \
#     --device /dev/snd \
#     -v /usr/share/alsa:/usr/share/alsa \
#     -v /etc/asound.conf:/etc/asound.conf \
#     -v ~/.asoundrc:/root/.asoundrc \
#     -e DISPLAY=$DISPLAY \
#     -v /tmp/.X11-unix:/tmp/.X11-unix \
#     -e XDG_RUNTIME_DIR=/tmp/xdg_runtime_dir \
#     --tmpfs /tmp/xdg_runtime_dir:rw,mode=0700 \
#     --cap-add=sys_nice \
#     --cap-add=ipc_lock \
#     $IMAGE_NAME ./build/Box_artefacts/Box
