#include "ArgParser.h"

#include <iostream>
#include <list>
#include <vector>

int main(int argc, const char** argv)
{
    bool flag = true;
    std::string outfilename = "output.txt";
    size_t size = 256;
    float x = 0;
    int n = 1, m = 0;

    arg::Parser<std::string, true> parser(
        "A configurable application\n"
        "Author: Gabor Borbely, Contact: borbely@math.bme.hu\n"
        "Very long text will be sliced into 'width' long chunks. But a manual '\\n' will produce a new-line, too.",  // header
        arg::list({"-h", "--help"}) /*help options*/,
        std::cout, std::cerr,  /*you have to specify the output streams! */
        "So Long, and Thanks for All the Fish!" /*optional footer*/,
        75 /*optional width*/);

    std::list<int> c1 = { 1, 2, 4, 8, 16 };
    parser.AddArg(n, arg::list({ "-n" }), "a number\nthe default value should match the choices, since the argument is optional", "", c1);

    parser.AddArg(m, arg::list({ }), "an other number\nIf the argument is positional, then the default value doesn't have to match the choices, because the check will be done when the argument will be provided", "", c1);
    
    parser.AddArg(outfilename, arg::list({"-o", "--output"}), "insert explanation here", "string");
    
    // explicit template parameter specification
    parser.AddArg<std::string, std::initializer_list<std::string>, arg::list>(outfilename, { }, "positional string", "string",
        { "a", "b", "c", "d", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p" });
    
    parser.AddArg(x, arg::list({}), "a real number, user must provide it");
    {
    std::vector<std::string> xflags = { "-x", "--float" };
    parser.AddArg(x, xflags, "a float number", "", -10.f, 10.f);
    }
    parser.AddArg(size, arg::list({ "-s", "--size" }), "a size, which can be very big!\n"
        "Due to unsigned type, '-1' reads as maximum positive value!");
    
    parser.AddFlag(flag, arg::list({ "-f", "--flag", "--set" }));
    parser.AddFlag(flag, arg::list({ "-F", "--reset" }), "", true, "negate");
 
    // parser.AddFlag(flag2, { }, "a positional flag is prohibited");
    parser.AddArg(flag, arg::list({ }), "you can make a positional flag like this", "logical");
    parser.AddArg(flag, arg::list({ "-b", "--bool" }), "this type of bool option is different from 'flag' and requires a value", "logical");
    
    if (parser.Do(argc, argv))
    {
        std::cout << m << std::endl;
        std::cout << outfilename << std::endl;
        std::cout << x << std::endl;
        std::cout << flag << std::endl;
        
        std::cout << std::endl;
        
        std::cout << n << std::endl;
        std::cout << size << std::endl;
        
        return 0;
    }
    else
    {
        std::cout << "Something failed!" << std::endl;
        return 1;
    }
}
