FROM debian:latest

# Install required tools
RUN apt update && apt install -y \
    build-essential \
    qemu-system-x86 \
    nasm \
    xorriso \
    grub-common \
    git \
    wget \
    curl \
    && rm -rf /var/lib/apt/lists/*

# Set up workspace
WORKDIR /os

# Default to interactive shell
CMD ["/bin/bash"]
