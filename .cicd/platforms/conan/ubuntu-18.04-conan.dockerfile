FROM ubuntu:18.04
ENV VERSION 1
ENV PATH="/usr/local/cmake/bin:${PATH}"

COPY ./scripts/pinned_toolchain.cmake /tmp/pinned_toolchain.cmake
RUN apt-get update && apt-get upgrade -y
