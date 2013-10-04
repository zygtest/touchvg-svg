#!/bin/sh
#
if [ ! -f svgimport_java_wrap.cpp ]; then # Make JNI classes
    mkdir -p ../src/svgimport/core
    rm -rf ../src/svgimport/core/*.*
    
    swig -c++ -java -package svgimport.core -D__ANDROID__ \
        -outdir ../src/svgimport/core \
        -o svgimport_java_wrap.cpp \
        -I../../../core/pub_inc \
        -I../../../core/pub_inc/cmd \
        -I../../../core/svgimport/gate \
        ../../../core/svgimport/gate/svgimport.i
fi
ndk-build # Make libsvgimport.so
