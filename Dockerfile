# Use an official Ubuntu as a parent image
FROM ubuntu:22.04

# Set environment variables
ENV DEBIAN_FRONTEND=noninteractive

# Install dependencies
RUN apt-get update && apt-get install -y software-properties-common && \
    apt-get update && apt-get install -y \
    clang \
    build-essential \
    wget \
    libglfw3-dev \
    libx11-dev \
    libxcursor-dev \
    libxrandr-dev \
    libxinerama-dev \
    libxi-dev \
    libxext-dev \
    libxfixes-dev \
    libwayland-dev \
    libxkbcommon-dev \
    pkg-config \
    libfreetype6-dev \
    freeglut3-dev \
    libasound2-dev \
    libcurl4-openssl-dev \
    libjack-jackd2-dev \
    libxcomposite-dev \
    mesa-common-dev \
    ladspa-sdk \
    webkit2gtk-4.0 \
    libgtk-3-dev \
    xvfb \
    ninja-build

RUN clang --version
   
# Install a newer version of CMake
RUN wget https://github.com/Kitware/CMake/releases/download/v3.22.0/cmake-3.22.0-linux-x86_64.sh && \
    chmod +x cmake-3.22.0-linux-x86_64.sh && \
    ./cmake-3.22.0-linux-x86_64.sh --skip-license --prefix=/usr/local && \
    rm cmake-3.22.0-linux-x86_64.sh

# Set the working directory
WORKDIR /usr/src/app

# Copy the current directory contents into the container at /usr/src/app
COPY . .

# Build the project (with half available cores)
RUN rm -rf build && cmake -S . -B build && cmake --build build --parallel $((`nproc` / 2))

# Run the executable
CMD ["./build/Box_artefacts/Box"]
