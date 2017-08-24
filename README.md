# Hane [![Build Status](https://travis-ci.org/sticnarf/hane.svg?branch=master)](https://travis-ci.org/sticnarf/hane)

Hane（羽根）is a simple web development framework.

~~It is my C++ curriculum design.~~

**Warning: DO NOT use it for production!**

## Features

* A built-in basic HTTP server
* Useful middleware for convenient web development
* Singular "Embedded C++" (ECP) for templates

## Usage

The project is compiled into a shared library by default.

You need to install [libuv](https://github.com/libuv/libuv) and [spdlog](https://github.com/gabime/spdlog)
before compiling this project.

```
$ mkdir build && cd build
$ cmake -DCMAKE_BUILD_TYPE=RELEASE ..
$ make
$ (sudo) make install
```

This will install the shared library and the header files, plus an executable `ecpc` (the template compiler).

You shall link the library to your application.

## Example

An example project `msgboard` is provided.

You can check `example/msgboard` and see how it works.

Also, this example project is dockerized. So, just have a try if you want! 

First, build the base image of the library `hane`:

```
# docker build -t libhane .
```

Then, change to the example directory, and run docker compose:

```
# cd examples/msgboard
# docker-compose up
```

Now, you can access the website: http://localhost:18089/

## Todo

Check [Issues](https://github.com/sticnarf/hane/issues).

## Built with

* [libuv](https://github.com/libuv/libuv)
* [spdlog](https://github.com/gabime/spdlog)

## Copyright

See [Copyright](https://github.com/hane/Copyright).