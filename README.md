libwire
=========

[![Travis CI](https://img.shields.io/travis/foxcpp/wire.svg?style=flat-square)](https://travis-ci.org/foxcpp/wire)
[![Latest Release](https://img.shields.io/github/release/foxcpp/wire.svg?style=flat-square)](https://github.com/foxcpp/wire/releases/latest)
[![Issues](https://img.shields.io/github/issues-raw/foxcpp/wire.svg?style=flat-square)](https://github.com/foxcpp/wire/issues)
[![License](https://img.shields.io/github/license/foxcpp/wire.svg?style=flat-square)](https://github.com/foxcpp/wire/blob/master/LICENSE)

Next-generation C++ networking library.
This repository contains implementation of platform-independent wrapper
on top system TCP & UDP implementation.

### Table of contents
* [Installation](#installation)
* [Usage](#usage)
* [Examples](#examples)
* [Documentation](#documentation)
* [Contributing](#contributing)
* [License](#license)

### Installation

#### Requirements
* C++17 compatible compiler
* _(Optional)_ Google Test for tests (included as submodule)

```
$ git clone https://github.com/foxcpp/wire
$ cd wire
$ mkdir build; cd build
$ cmake .. -DCMAKE_BUILD_TYPE=Release
$ cmake --build .
```

**Note 1** By default libwire will be built as a static library, if you want
a shared library then you should add `-DBUILD_SHARED_LIBS=ON` flag.

### Usage

#### Build system configuration

If you use CMake and clone libwire as a submodule, you can just link it this way:
```cmake
add_subdirectory(wire/)
target_link_library(your-exectuable libwire)
```
It will use CMake magic to configure include path and other stuff.

Alternatively, you can access system-installed version using `find_package`.
```cmake
find_package(libwire REQUIRED)
target_link_library(your-executable libwire)
```


#### In code

Let's write a simple TCP server that echoes back `\n`-terminated strings.

_We can't show super-simple example to surprise you, see more complicated_
_but complete examples in [examples/](examples/) directory._


### Examples

See [examples/](examples/) directory for sources and [documentation][docs] for detailed description.


### Documentation

Maintained in Markdown format (for separate page) and as a docstrings in code (for reference).
Documentation for lastest commit in master is availiable [here][docs].
You can also build it locally by running `make doc`. Ouput will be placed in `doxygen_output/` directory.

**Note** Undocumented public function which behavior is not obvious is A BUG. Report it!

### Contributing

Here is some of the ways that you can contribute:
* Submit a bug report. We love hearing about broken things, so that we can fix them.
* Provide feedback. Even simple questions about how things work or why they were done a certain way carries value.
* Test code. Checkout `dev` branch, compile it and start trying to break stuff! :-)
* Write code. Check issues marked with [**help wanted**](https://github.com/foxcpp/wire/issues?q=is%3Aissue+is%3Aopen+label%3A%22help+wanted%22) tag.

See [CODE_STYLE.md](CODE_STYLE.md) and [CONTRIBUTING.md](CONTRIBUTING.md) for more details.
Use [Uncrustify](http://uncrustify.sourceforge.net/) for auto-formatting.
(there is no configuration file for it yet, sorry, but it's not possible to configure different formatters for 100% match with our codestyle)

### License

Distributed under terms of the MIT license. It allows you to do anything with
libwire as long as you don't remove any copyright notices. Also, there
is no warranties about anything.



[docs]: https://foxcpp.github.io/wire
