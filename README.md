# ArgParser
[argparse](https://docs.python.org/3/library/argparse.html)-like command line argument parsing in c++

* header-only
  * include a single header file
  * with binary examples 
* requires c++11 or Visual Studio 2017 or later
* build with [CMake](https://cmake.org/)

## Features
* handles non-ascii command line inputs
  * on Linux, just use UTF-8
  * on Windows you can use wide-char-strings
  * although, printing of `std::wstring` variables is not trivial (see example)
* custom conditions can be applied to options
  * min-max bounds
  * choice from a predefined set
  * any bool-valued function can be provided

