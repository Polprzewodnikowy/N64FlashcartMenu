FROM ghcr.io/n64-tools/gcc-toolchain-mips64:latest

RUN apt-get update && \
    apt-get install -y \
    python3 \
    python3-serial \
    python3-pil

RUN SNIPPET="export PROMPT_COMMAND='history -a' && export HISTFILE=/commandhistory/.bash_history" \
    && echo "$SNIPPET" >> "/root/.bashrc"

# TODO: Really we want to use the sub module in /workspaces/N64FlashcartMenu/libdragon
RUN git clone --branch trunk https://github.com/dragonminded/libdragon.git --depth 1 ./sources/libdragon

RUN cd ./sources/libdragon && \
    make -j libdragon && \
    make install && \
    make -j tools && \
    make tools-install && \
    make install-mk
