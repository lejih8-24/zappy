# Zappy
Zappy is an Epitech project simulating fight between monsters

Its a really cool project trust me.

## Building the project

Zappy uses [CMake](https://cmake.org/) as its primary
build tool. In order to use it, it's highly recommended
you create a `build/` directory first:

```bash
mkdir build
```

### First build

When building Zappy for the first time, you need to first
generate the relevant CMake files. It's highly recommended
you use the release build for this:

```bash
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
```

> [!INFO]
> If disk space is a concern to you,
> you may alternatively use the *minimum size release*
> build type. This is not usually recommended, since
> the compiled files don't take a lot of disk space
> by themselves, and having a minimum size release
> may decrease the overall performance of the Zappy.
> Nonetheless, if you require it, you may use the following
> command:
> `cmake -DCMAKE_BUILD_TYPE=MinSizeRel ..`

After generating the cmake files, you may proceed to the
next step below.

### Subsequent builds

To actually build the project's exectuables, all you
need to do is run the following command in the `build/`
directory:

``` bash
cmake --build .
```

The compiled binaries will be in the `build/` directory,
meaning you'll be able to run `./zappy_ai`, `./zappy_server`,
and `./zappy_gui`.

