FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y software-properties-common && \
    apt-get update && apt-get install -y \
    libglfw3 libglfw3-dev libxfixes-dev libwayland-dev libxkbcommon-dev freeglut3-dev g++ libasound2-dev libcurl4-openssl-dev libfreetype6-dev libjack-jackd2-dev libxcomposite-dev mesa-common-dev ladspa-sdk webkit2gtk-4.0 libgtk-3-dev xvfb \
    alsa-utils jackd2 qjackctl vim cmake clang && \
    apt-get clean && rm -rf /var/lib/apt/lists/*

# Add root to audio group
RUN groupadd -f audio && usermod -aG audio root

# Set limits for both audio group and root user
RUN echo "@audio - rtprio 95" >> /etc/security/limits.d/audio.conf && \
    echo "@audio - memlock unlimited" >> /etc/security/limits.d/audio.conf && \
    echo "root - rtprio 95" >> /etc/security/limits.d/audio.conf && \
    echo "root - memlock unlimited" >> /etc/security/limits.d/audio.conf

# Configure shared memory
RUN mkdir -p /dev/shm && \
    echo "tmpfs /dev/shm tmpfs defaults,size=512M 0 0" >> /etc/fstab

# Copy entrypoint script
COPY entrypoint.sh /entrypoint.sh
RUN chmod +x /entrypoint.sh

ENV JACK_NO_AUDIO_RESERVATION=1

WORKDIR /usr/src/box
COPY . .

RUN rm -rf build && cmake -S . -B build && cmake --build build --parallel $((`nproc` / 2))

ENTRYPOINT ["/entrypoint.sh"]
