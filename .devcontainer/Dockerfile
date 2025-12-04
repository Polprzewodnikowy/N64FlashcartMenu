FROM debian:trixie-slim

ARG SC64_DEPLOYER_VERSION=v2.20.2

# Minimal image: install only what's needed for doc building and the project's toolchain
RUN apt-get update && \
    apt-get upgrade -y && \
    apt-get install -y build-essential doxygen git python3 wget

# Add Microsoft package repository and install .NET SDK (apt will pick latest patch)
#     apt-get install software-properties-common && \
RUN add-apt-repository ppa:dotnet/backports && \
    apt-get update
#     apt-get install -y dotnet-sdk-8.0

# Install libdragon toolchain (packaged .deb)
RUN wget https://github.com/DragonMinded/libdragon/releases/download/toolchain-continuous-prerelease/gcc-toolchain-mips64-x86_64.deb && \
    dpkg -i gcc-toolchain-mips64-x86_64.deb && \
    rm gcc-toolchain-mips64-x86_64.deb

# Install SummerCart64 deployer
RUN wget https://github.com/Polprzewodnikowy/SummerCart64/releases/download/$SC64_DEPLOYER_VERSION/sc64-deployer-linux-$SC64_DEPLOYER_VERSION.tar.gz && \
    tar -xf sc64-deployer-linux-$SC64_DEPLOYER_VERSION.tar.gz -C /usr/local/bin && \
    rm sc64-deployer-linux-$SC64_DEPLOYER_VERSION.tar.gz

# Misc config
RUN git config --global --add safe.directory "*" && \
    SNIPPET="export PROMPT_COMMAND='history -a' && export HISTFILE=/commandhistory/.bash_history" && \
    echo "$SNIPPET" >> "/root/.bashrc"

