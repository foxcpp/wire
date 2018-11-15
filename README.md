libwire
=========

### Project is not developed

**I'm not using C++ anymore and have zero interest in libwire development. All current code in libwire is working as it should, so you can use it if you really want but using https://github.com/irisjyf/evpp if you need something production-ready.**


[![Travis CI](https://img.shields.io/travis/foxcpp/wire.svg?style=flat-square&logo=Linux)](https://travis-ci.org/foxcpp/wire)
[![AppVeyor CI](https://img.shields.io/travis/foxcpp/wire.svg?style=flat-square&logo=Windows)](https://ci.appveyor.com/project/foxcpp/wire)
[![CodeCov](https://img.shields.io/codecov/c/github/foxcpp/wire.svg?style=flat-square)](https://codecov.io/gh/foxcpp/wire)
[![Latest Release](https://img.shields.io/github/release/foxcpp/wire.svg?style=flat-square)](https://github.com/foxcpp/wire/releases/latest)
[![Issues](https://img.shields.io/github/issues-raw/foxcpp/wire.svg?style=flat-square)](https://github.com/foxcpp/wire/issues)
[![License](https://img.shields.io/github/license/foxcpp/wire.svg?style=flat-square)](https://github.com/foxcpp/wire/blob/master/LICENSE)

C++ networking library.
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

  **MSVC is not supported currently due to cryptic errors. If you know how to fix them - please, send PR.**

* _(Optional)_ Google Test for tests (included as submodule)
* _(Optional)_ Doxygen for API documentation generation

```
$ git clone https://github.com/foxcpp/wire
$ cd wire
$ mkdir build; cd build
$ cmake .. -DCMAKE_BUILD_TYPE=Release
$ cmake --build .
```

**Note 1** By default libwire will be built as a static library, if you want
a shared library then you should add `-DBUILD_SHARED_LIBS=ON` flag.

**Note 2** It's recommended to enable LTO in your compiler to allow
cross-object inlining and stuff.


### Usage

#### Build system configuration

If you use CMake and clone libwire as a submodule, you can just link it this way:
```cmake
add_subdirectory(wire/)
target_link_libraries(your-exectuable libwire)
```
It will use CMake magic to configure include path and other stuff.

Alternatively, you can access system-installed version using `find_package`.
```cmake
find_package(libwire REQUIRED)
target_link_libraries(your-executable libwire)
```


#### In code

Let's write a simple TCP server that echoes back `\n`-terminated strings.

First, include TCP listener header and another one for `ipv4::any`:
```cpp
#include <libwire/tcp/listener.hpp>
#include <libwire/address.hpp>
```

Then... It's very short so it doesn't needs any commentary:
```cpp
using namespace libwire;

tcp::listener listener({ipv4::any, 7});

while (true) {
    auto sock = listener.accept();
    auto str = sock.read_until<std::string>('\n');
    sock.write(str);
}
```


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

### License

Distributed under terms of the MIT license. It allows you to do anything with
libwire as long as you don't remove any copyright notices. Also, there
is no warranties about anything.

See [LICENSE.md](LICENSE.md) for details.



[docs]: https://foxcpp.github.io/wire
