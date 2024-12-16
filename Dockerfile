# Use an official Ubuntu as a parent image
FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y software-properties-common && \
    # TE and raylib deps
    apt-get update && apt-get install -y \
    libglfw3 libglfw3-dev libxfixes-dev libwayland-dev libxkbcommon-dev freeglut3-dev g++ libasound2-dev libcurl4-openssl-dev libfreetype6-dev libjack-jackd2-dev libxcomposite-dev mesa-common-dev ladspa-sdk webkit2gtk-4.0 libgtk-3-dev xvfb && \
    # Audio deps
    apt-get install -y \
    alsa-utils jackd2 qjackctl && \

    apt-get install -y \
    vim

# Add user and group for audio, only if they don't exist

RUN groupadd -f audio && usermod -aG audio root

RUN echo "@audio - rtprio 95" >> /etc/security/limits.d/audio.conf && \
    echo "@audio - memlock unlimited" >> /etc/security/limits.d/audio.conf

# Allow memory lock and CAP_SYS_NICE for the container
RUN mkdir -p /dev/shm && \
    echo "tmpfs /dev/shm tmpfs defaults,size=512M 0 0" >> /etc/fstab

# # Set the working directory
# WORKDIR /usr/src/app

# # Copy the current directory contents into the container at /usr/src/app
# COPY . .

# # Build the project
# RUN rm -rf build && cmake -S . -B build && cmake --build build --parallel $((`nproc` / 2))

# # Configure ALSA (create default asoundrc)
# RUN echo "pcm.!default { type plug slave.pcm \"hw:0,0\" }" > ~/.asoundrc
# RUN groupadd -f audio && usermod -aG audio root

# # Disable PulseAudio and set up JACK within the container
# RUN systemctl --user disable pulseaudio.socket pulseaudio.service && \
#     apt-get remove -y pulseaudio && \
#     apt-get clean

# # Configure entrypoint for audio setup and app launch
# COPY entrypoint.sh /usr/local/bin/entrypoint.sh
# RUN chmod +x /usr/local/bin/entrypoint.sh

# # Entrypoint to start JACK and run the app
# ENTRYPOINT ["/usr/local/bin/entrypoint.sh"]
