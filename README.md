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

An example project called `hello` that simply greets is provided.

You can check `example/hello` and see how it works.

Also, a `Dockerfile` for `hello` is provided. You can build the image using: 

```
# docker build -t hane_hello .
```

And run it in a container:

```
# docker run -d -v /tmp:/var/log -p 80:8089 hane_hello
```

Now, you can access the website through port `80`:

```
$ curl http://localhost/?name=Alice
<!DOCTYPE HTML>
<title>Hello</title>
<meta charset="UTF-8">
<h1>Hello, Alice!</h1>
```

## Todo

Check [Issues](https://github.com/sticnarf/hane/issues).

## Acknowledges

* [libuv](https://github.com/libuv/libuv)
* [spdlog](https://github.com/gabime/spdlog)
* `cmake/FindLibUV.cmake` from [neovim/neovim](https://github.com/neovim/neovim/blob/53b38251bb0bfdb1d002b09001417e708d85e422/cmake/FindLibUV.cmake)
* `cmake/Findspdlog.cmake` from [UCL/GreatCMakeCookOff](https://github.com/UCL/GreatCMakeCookOff/blob/4be3c59d2ea42d5f094c481d75e4944f6bf8c127/modules/Findspdlog.cmake)