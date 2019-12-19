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
* `strict` option
  * if set, the parser calls `exit(1)` on any trouble
  * if not set, then return value and error message tells you the problems, but the application continues.
    * in case of any trouble, your initial variables won't be changed. No incomplete state in the options!

## Examples
Minimal arguments:
```C++
int main(int argc, const char** argv)
{
    double x;
    std::string s;

    arg::Parser<> parser("Header, some info",
        { "-h", "--help" } /*help options*/,
        std::cout, std::cerr  /*you have to specify the output streams! */
    );
    parser.AddArg(x);
    parser.AddArg(s, { "-s", "--string" }, "relative path", "filename");

    parser.Do(argc, argv);
    
    return 0;
}
```
And you're done!
