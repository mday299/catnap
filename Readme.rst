Catnap (C/C++ REST)
===================

.. image:: https://github.com/cweidner3/catnap/workflows/CMake/badge.svg
   :alt: CMake Workflow Status

Library intended to provide a simplified interface for a REST API control layer
for C/C++ applications. This intends to do this with the use of `Boost`_'s
Beast library for the HTTP server implementation and `nlohmann_json`_. These
dependencies should ideally not be required to be installed seaparately unless
specifically compiled for these to be shared.

.. _nlohmann_json: https://github.com/nlohmann/json
.. _Boost: https://www.boost.org/

Dependencies
------------

- C++ Compiler (*gcc or clang*)
- Conan (*avaliable via pip*)

Third Party Libraries handled by Conan.

+-----------------------+---------+------------------+
| Dependency            | Version | Mode             |
+=======================+=========+==================+
| Boost_ (Beast & ASIO) | 1.69.0  | Static           |
+-----------------------+---------+------------------+
| nlohmann_json_        |         | Shared or Static |
+-----------------------+---------+------------------+

Building
--------

.. code:: shell

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

.. code:: shell

    python3 ./make.py setup --clang

Coverage
--------

Coverage requires either llvm tools when using Clang, or gcov (*provided with
gcc*) and lcov when using gcc compiler.

To enable coverage reporting certain compiler flags are needed, use the
``--coverage`` flag to set these when setting up the makefile structure.

.. code:: shell

    python3 ./make.py setup --coverage

For both toolings, the unit tests need to run to generate data files. For the
clang tooling the ``coverage`` target can be run.

.. code:: shell

    cd build
    cmake --build . --target coverage
