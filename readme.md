## `simple_geo_reader`

Most simple node that sensibly outputs geo.

A simplified version of the Sphere.cpp NDK example

    cd simple_geo_reader
    mkdir build && cd build

    cmake -D NUKE_INSTALL_PATH=/Applications/Nuke6.2v2/Nuke6.2v2.app/Contents/MacOS/ -D CMAKE_INSTALL_PREFIX=../dist ..

    make
    make install
