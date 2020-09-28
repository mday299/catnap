Catnap (C/C++ REST)
===================

![CMake](https://github.com/cweidner3/catnap/workflows/CMake/badge.svg)


Library intended to provide a simplified interface for a REST API control layer
for C/C++ applications. This intends to do this with the use of [Boost][]'s
Beast library for the HTTP server implementation and [nlohmann_json][]. These
dependencies should ideally not be required to be installed seaparately unless
specifically compiled for these to be shared.

[nlohmann_json]: https://github.com/nlohmann/json
[Boost]: https://www.boost.org/
[CMake]: https://cmake.org/

Dependencies
------------

- C++ Compiler (*gcc or clang*)
- [CMake][]
- Conan (*avaliable via pip*)

Third Party Libraries handled by Conan.

| Dependency               | Version | Mode             |
|--------------------------|---------|------------------|
| [Boost][] (Beast & ASIO) | 1.69.0  | Static           |
| [nlohmann_json][]        |         | Shared or Static |

Building
--------

    # To install conan
    pip install --user conan
    # Initialize the ~/.conan directoryo
    # and ensure libstdc++11 is used for later versions of GCC and Clang
    conan config init
    conan profile update settings.compiler.libcxx=libstdc++11 default
    # Setup build
    python3 ./make.py setup
    # Build
    python3 ./make.py build

Using Clang Tools
-----------------

    python3 ./make.py setup --clang

Coverage
--------

Coverage requires either llvm tools when using Clang, or gcov (*provided with
gcc*) and lcov when using gcc compiler.

To enable coverage reporting certain compiler flags are needed, use the
``--coverage`` flag to set these when setting up the makefile structure.

    python3 ./make.py setup --coverage

For both toolings, the unit tests need to run to generate data files. For the
clang tooling the ``coverage`` target can be run.

    cd build
    cmake --build . --target coverage
