docker run --rm -it \
    --privileged \
    --ulimit memlock=-1:-1 \
    --cap-add=sys_nice \
    --device /dev/snd \
    audio bash
