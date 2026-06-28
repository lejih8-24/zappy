# Zappy
Zappy is an Epitech project simulating fight between monsters

Its a really cool project trust me.

## Cloning the repository

Zappy uses [git submodules](https://git-scm.com/book/en/v2/Git-Tools-Submodules) for
its dependencies, meaning you need to tell git to also clone the submodules to
ensure you can compile the project.
To do so, all you need to do is add the `--recursive` flag when cloning:

``` bash
git clone --recursive https://github.com/LeJih8-24/Zappy.git
```

> [!TIP]
> If you forgot to add the `--recursive` flag
> when cloning, or if a dependency was updated
> since you cloned, you can run `git submodule update --init`
> to update the submodules.

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


## Running the programs

Once you've built the project, you can
run the programs, which will all be located
in the build directory you created.

### Zappy GUI and Server

Both the GUI and server can be run seemlessly
by executing the binaries with the appropriate
arguments (for more information on those, please
refer to the respective program's help page or documentation).

Example:

``` bash
./zappy_server -p 5000 -x 10 -y 10 -n A B -c 3
```

> [!NOTE]
> Running the above will start the zappy
> server on port 5000, if possible.

### Zappy AI

The AI implementation for the Zappy project has some Python dependencies.
As such, it's recommended you use a *virtual environment* (`venv` henceforth).

In order to set one up in the build directory, all you need to do is run:
``` bash
python3 -m venv venv
```

> [!NOTE]
> This will create a directory called `venv`
> containing all files required for the environment.

To activate it, all you need to do is run:

``` bash
source venv/bin/activate
```

and install the required dependencies:

``` bash
pip install -r ../ai/requirements.txt
```

After that, you may then run the AI like
you usually would:

``` bash
./zappy_ai -n MyTeam -p 5000
```

