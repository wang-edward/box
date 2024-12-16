docker run --rm -it \
    --privileged \
    --ulimit memlock=-1:-1 \
    --cap-add=sys_nice \
    --cap-add=IPC_LOCK \
    --ipc=host \
    --device /dev/snd \
    --security-opt seccomp=unconfined \
    --security-opt apparmor=unconfined \
    audio
