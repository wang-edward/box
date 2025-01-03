FROM ghcr.io/wang-edward/box-base:latest

WORKDIR /usr/src/app

COPY . .

RUN cmake -S . -B build && cmake --build build --parallel $(nproc)

# CMD ["./build/Box_artefacts/Box"]
