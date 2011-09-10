## `simple_geo_reader`

Most simple node that sensibly outputs geo.

A simplified version of the Sphere.cpp NDK example

    cd simple_geo_reader
    mkdir build && cd build

    cmake -D NUKE_INSTALL_PATH=/Applications/Nuke6.2v2/Nuke6.2v2.app/Contents/MacOS/ -D CMAKE_INSTALL_PREFIX=../dist ..

    make
    make install


## `nuke_verbosity`

A compiled Python module which exposes the "nuke -V" flag

    $ nuke -t
    >>> import nuke_verbosity
    >>> nuke_verbosity.is_verbose()
    False
    $ nuke -t -V
    >>> import nuke_verbosity
    >>> nuke_verbosity.is_verbose()
    True


For example, this could be used to control the python logging level:

    if nuke_verbosity.is_verbose():
        logging.setLevel(logging.DEBUG)
    else:
        logging.setLevel(logging.INFO)


To build:

    cd nuke_verbosity
    mkdir build && cd build

    cmake -D NUKE_INSTALL_PATH=/Applications/Nuke6.3v1/Nuke6.3v1.app/Contents/MacOS/ -D CMAKE_INSTALL_PREFIX=../dist ..

    make
    make install

    env PYTHONPATH=../dist/lib/nuke6.3 /Applications/Nuke6.3v1/Nuke6.3v1.app/Contents/MacOS/Nuke6.3v1
