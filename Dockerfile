# for build this code project
FROM ubuntu:20.04
# set workspace dir
WORKDIR /workspace
# copy code to workspace
COPY . .
# install isstenal build component
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update -y && \
    apt-get install -y build-essential cmake qt5-default qtbase5-dev && \
    apt-get clean all
# build code project
RUN ["cmake" "-B" "/workspace/build" "-DCMAKE_BUILD_TYPE=Release" "-DQT_ROOT_DIR=/usr/lib/x86_64-linux-gnu/qt5"]
RUN ["cd" "build"]
RUN ["make"]