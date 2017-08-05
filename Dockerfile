FROM debian:stable-slim

LABEL maintainer="sticnarf@gmail.com"

RUN apt-get update \
 && apt-get install -y build-essential libspdlog-dev libuv1 libuv1-dev cmake

WORKDIR /work

COPY . .

RUN mkdir build \
 && cd build \
 && cmake -DCMAKE_BUILD_TYPE=RELEASE .. \
 && make -j2 \
 && make install

WORKDIR /work/examples/hello

RUN mkdir build \
 && cd build \
 && cmake -DCMAKE_BUILD_TYPE=RELEASE .. \
 && make -j2 \
 && make install \
 && ldconfig
# && cd / \
# && rm -rf work \
# && apt-get remove --purge -y build-essential libspdlog-dev libuv1-dev cmake \
# && apt-get autoremove --purge -y \
# && rm -rf /var/lib/apt/lists/*

VOLUME /var/log
EXPOSE 8089

ENTRYPOINT "hello"
