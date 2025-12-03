FROM debian:bookworm-slim

ARG SC64_DEPLOYER_VERSION=v2.20.2

ENV DEBIAN_FRONTEND=noninteractive
ENV PATH="$PATH:/root/.dotnet/tools"

# Minimal image: install only what's needed for doc building and the project's toolchain
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        ca-certificates wget gnupg curl apt-transport-https unzip git && \
    # Add Microsoft package repository and install .NET SDK 7.0 (apt will pick latest patch)
    wget https://packages.microsoft.com/config/debian/12/packages-microsoft-prod.deb -O packages-microsoft-prod.deb && \
    dpkg -i packages-microsoft-prod.deb && rm packages-microsoft-prod.deb && \
    apt-get update && \
    apt-get install -y --no-install-recommends dotnet-sdk-7.0 && \
    # Install docfx as a dotnet global tool
    dotnet tool install -g docfx && \
    ln -sf /root/.dotnet/tools/docfx /usr/local/bin/docfx || true && \
    # Install libdragon toolchain (packaged .deb)
    wget https://github.com/DragonMinded/libdragon/releases/download/toolchain-continuous-prerelease/gcc-toolchain-mips64-x86_64.deb -O /tmp/gcc-toolchain.deb && \
    dpkg -i /tmp/gcc-toolchain.deb || true && rm /tmp/gcc-toolchain.deb && \
    # Install SummerCart64 deployer
    wget https://github.com/Polprzewodnikowy/SummerCart64/releases/download/$SC64_DEPLOYER_VERSION/sc64-deployer-linux-$SC64_DEPLOYER_VERSION.tar.gz -O /tmp/sc64.tar.gz && \
    tar -xf /tmp/sc64.tar.gz -C /usr/local/bin && rm /tmp/sc64.tar.gz || true && \
    # Misc config
    git config --global --add safe.directory "*" && \
    echo "export PROMPT_COMMAND='history -a' && export HISTFILE=/commandhistory/.bash_history" >> /root/.bashrc && \
    apt-get clean && rm -rf /var/lib/apt/lists/*

