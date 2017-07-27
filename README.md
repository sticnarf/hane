# Rackcpp [![Build Status](https://travis-ci.org/sticnarf/rackcpp.svg?branch=master)](https://travis-ci.org/sticnarf/rackcpp)

Under development...

## Usage

A example project called `hello` that simply greets is provided.

You can check `example/hello` and see how it works.

Also, a `Dockerfile` for `hello` is provided. You can build the image using: 

```
# docker build -t rackcpp_hello .
```

And run it in a container:

```
# docker run -d -v /tmp:/var/log -p 80:8089 rackcpp_hello
```

Now, you can access the website through port `80`:

```
$ curl http://localhost/?name=Alice
<!DOCTYPE HTML>
<title>Hello</title>
<meta charset="UTF-8">
<h1>Hello, Alice!</h1>
```

## Acknowledges

* [libuv](https://github.com/libuv/libuv)
* `cmake/FindLibUV.cmake` from [neovim/neovim](https://github.com/neovim/neovim/blob/53b38251bb0bfdb1d002b09001417e708d85e422/cmake/FindLibUV.cmake)
* `cmake/Findspdlog.cmake` from [UCL/GreatCMakeCookOff](https://github.com/UCL/GreatCMakeCookOff/blob/4be3c59d2ea42d5f094c481d75e4944f6bf8c127/modules/Findspdlog.cmake)