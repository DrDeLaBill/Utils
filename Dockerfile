ARG CI_REGISTRY=registry.izhpt.com:443

FROM $CI_REGISTRY/ayratproject/stm_builder:main AS builder

ENV APP_ROOT=/app
ENV SRC_ROOT=$APP_ROOT/src
ENV BUILD_ROOT=$APP_ROOT/build

ADD . $SRC_ROOT
RUN mkdir -p $BUILD_ROOT \
 && cp -f $APP_ROOT/search.cmake $SRC_ROOT/search.cmake

RUN cd $BUILD_ROOT \
 && cmake -G"Unix Makefiles" $SRC_ROOT -DCMAKE_BUILD_TYPE=Debug -DDEBUG=1 \
 && cmake --build $BUILD_ROOT

RUN $BUILD_ROOT/test/utilstest
