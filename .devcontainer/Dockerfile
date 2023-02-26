FROM ghcr.io/n64-tools/gcc-toolchain-mips64:latest

# Avoid warnings by switching to noninteractive
ENV DEBIAN_FRONTEND=noninteractive

RUN git clone --branch trunk https://github.com/dragonminded/libdragon.git --depth 1 ./sources/libdragon

RUN cd ./sources/libdragon && \
    make -j libdragon && \
    make install && \
    make -j tools && \
    make tools-install && \
    make install-mk
