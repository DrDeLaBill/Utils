ARG BASE_IMAGE=gcc:12.2.0-bullseye

FROM $BASE_IMAGE AS builder

ENV APP_ROOT=/app
ENV SRC_ROOT=$APP_ROOT/src
ENV DEBUG_ROOT=$APP_ROOT/Debug
ENV RELEASE_ROOT=$APP_ROOT/Release

ADD . $SRC_ROOT

RUN apt-get update && apt-get install -y cmake && rm -rf /var/lib/apt/lists/*

RUN mkdir -p $DEBUG_ROOT \
 && mkdir -p $RELEASE_ROOT

RUN cd $DEBUG_ROOT \
 && cmake $SRC_ROOT -DCMAKE_BUILD_TYPE=DEBUG -DDEBUG=1 \
 && cmake --build $DEBUG_ROOT \
 && $DEBUG_ROOT/test/utilstest

RUN cd $RELEASE_ROOT \
 && cmake $SRC_ROOT \
 && cmake --build $RELEASE_ROOT \
 && $RELEASE_ROOT/test/utilstest