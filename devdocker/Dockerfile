FROM ubuntu:18.04

# Pre-install curl to setup bazel apt-get repo.
RUN apt-get update && apt-get install -y curl software-properties-common
RUN echo "deb [arch=amd64] " \
      "http://storage.googleapis.com/bazel-apt stable jdk1.8" | \
      tee /etc/apt/sources.list.d/bazel.list
RUN curl https://bazel.build/bazel-release.pub.gpg | apt-key add -

# Basic dev environment (c++, bazel, boost)
RUN apt-get update && apt-get install -y \
      bazel \
      build-essential \
      gdb \
      git \
      libboost-all-dev \
      man-db \
      python \
      python-pip \
      sudo \
      vim
RUN pip install pyyaml

COPY 3rdparty/3rdparty /3rdparty/

# Install googletest
COPY 3rdparty/googletest /devdocker/3rdparty/googletest/
RUN /3rdparty/3rdparty mklib googletest --cleanup
