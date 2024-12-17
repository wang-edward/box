xhost +local:root

docker run --rm -it \
    -e DISPLAY=$DISPLAY \
    -v /tmp/.X11-unix:/tmp/.X11-unix \
    --privileged \
    --ulimit memlock=-1:-1 \
    --cap-add=sys_nice \
    --cap-add=IPC_LOCK \
    --ipc=host \
    --device /dev/snd \
    --security-opt seccomp=unconfined \
    --security-opt apparmor=unconfined \
    audio bash
