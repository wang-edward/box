#!/bin/bash

# Set variables
IMAGE_NAME="box"
CONTAINER_NAME="container"

# Build the Docker image
docker build -t $IMAGE_NAME .

# Run the Docker container with ALSA support
docker run --rm -it \
  --name $CONTAINER_NAME \
  --device /dev/snd \
  -v /usr/share/alsa:/usr/share/alsa \
  -v /etc/asound.conf:/etc/asound.conf \
  -v ~/.asoundrc:/root/.asoundrc \
  $IMAGE_NAME
