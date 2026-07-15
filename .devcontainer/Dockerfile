# Use the modern official Microsoft C++ Ubuntu 24.04 C++ base image
FROM mcr.microsoft.com/devcontainers/cpp:dev-ubuntu24.04

# This line is for ARM64, specifically on MacOS. If you don't need it, just comment it out (though it likely doesn't hurt you).
RUN if [ "$(uname -m)" = "aarch64" ]; then \
	echo "Executing ARM64 uid/gid modification"; \
	groupmod --gid 501 vscode && usermod --uid 501 --gid 501 vscode && chown -R 501:501 /home/vscode; \
	fi

RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install --no-install-recommends \
    gcc \
    g++ \
    git \
    gdb \
    clang \
    clang-tools \
    mariadb-server \
    libmariadb-dev \
    libmariadb-dev-compat \
    cmake \
    ninja-build \
    valgrind