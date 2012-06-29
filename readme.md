## `simple_geo_reader`

Most simple node that sensibly outputs geo.

A simplified version of the Sphere.cpp NDK example

    cd simple_geo_reader
    mkdir build && cd build

    cmake -D NUKE_INSTALL_PATH=/Applications/Nuke6.2v2/Nuke6.2v2.app/Contents/MacOS/ -D CMAKE_INSTALL_PREFIX=../dist ..

    make
    make install


## `nuke_verbosity`

A Python module which exposes the Nuke verbosity level. This isn't
necessary, as there is a simpler way of doing this with no compiling
required:

    import nuke
    if "-V" in nuke.rawArgs:
        print "Is verbose"

However this module might be a useful reference for doing similar
things. It works like this:

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

## `ratReader`

An incomplete deep shadow map/deep camera map reader

This will not work on OS X, due to incompatibilities between how
Houdini and Nuke are built,

http://www.sidefx.com/index.php?option=com_forum&Itemid=172&page=viewtopic&p=102608

## `dcm_convertor`

An even more incomplete tool to convert a Houdini's .rat deep
[camera/shadow] map to .dtex