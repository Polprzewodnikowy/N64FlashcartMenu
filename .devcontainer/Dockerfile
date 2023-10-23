FROM debian:bookworm-slim

RUN apt-get update && \
    apt-get upgrade -y && \
    apt-get install build-essential doxygen git python3 wget gdb-multiarch -y && \
    # apt-get install unzip usbutils -y && \
    wget https://github.com/DragonMinded/libdragon/releases/download/toolchain-continuous-prerelease/gcc-toolchain-mips64-x86_64.deb && \
    dpkg -i gcc-toolchain-mips64-x86_64.deb && \
    rm gcc-toolchain-mips64-x86_64.deb && \
    wget https://github.com/Polprzewodnikowy/SummerCart64/releases/download/v2.17.0/sc64-deployer-linux-v2.17.0.tar.gz && \
    tar -xf sc64-deployer-linux-v2.17.0.tar.gz -C /usr/local/bin && \
    rm sc64-deployer-linux-v2.17.0.tar.gz && \
    # wget https://github.com/buu342/N64-UNFLoader/releases/download/v2.1/UNFLoader-Linux-x64.zip && \
    # unzip UNFLoader-Linux-x64.zip -d /usr/local/unfloader && \
    # cp -f "/usr/local/unfloader/UNFLoader" "/usr/local/bin/UNFLoader"
    # yes Y | . /usr/local/unfloader/installer_linux.sh && \
    # rm UNFLoader-Linux-x64.zip && \
    # rm -rf /usr/local/unfloader && \
    git config --global --add safe.directory "*" && \
    SNIPPET="export PROMPT_COMMAND='history -a' && export HISTFILE=/commandhistory/.bash_history" && \
    echo "$SNIPPET" >> "/root/.bashrc"
