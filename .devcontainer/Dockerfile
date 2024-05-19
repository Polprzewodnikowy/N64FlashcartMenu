FROM debian:bookworm-slim

ARG SC64_DEPLOYER_VERSION=v2.18.0
RUN apt-get update && \
    apt-get upgrade -y && \
    apt-get install build-essential doxygen git python3 wget -y && \
    wget https://github.com/networkfusion/libdragon/releases/download/toolchain-continuous-prerelease/gcc-toolchain-mips64-x86_64.deb && \
    dpkg -i gcc-toolchain-mips64-x86_64.deb && \
    rm gcc-toolchain-mips64-x86_64.deb && \
    wget https://github.com/Polprzewodnikowy/SummerCart64/releases/download/$SC64_DEPLOYER_VERSION/sc64-deployer-linux-$SC64_DEPLOYER_VERSION.tar.gz && \
    tar -xf sc64-deployer-linux-$SC64_DEPLOYER_VERSION.tar.gz -C /usr/local/bin && \
    rm sc64-deployer-linux-$SC64_DEPLOYER_VERSION.tar.gz && \
    git config --global --add safe.directory "*" && \
    SNIPPET="export PROMPT_COMMAND='history -a' && export HISTFILE=/commandhistory/.bash_history" && \
    echo "$SNIPPET" >> "/root/.bashrc"
