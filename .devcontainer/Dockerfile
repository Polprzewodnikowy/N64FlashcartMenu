FROM ghcr.io/n64-tools/gcc-toolchain-mips64:latest

RUN apt-get update && apt-get install -y python3 python3-serial python3-pil

RUN mkdir -p /etc/udev/rules.d
RUN echo 'KERNEL=="ttyUSB[0-9]*",MODE="0666' > /etc/udev/rules.d/99-serial.rules

RUN git clone --branch trunk https://github.com/dragonminded/libdragon.git --depth 1 ./sources/libdragon

RUN cd ./sources/libdragon && \
    make -j libdragon && \
    make install && \
    make -j tools && \
    make tools-install && \
    make install-mk
