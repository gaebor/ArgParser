
#define NOMINMAX
#include <Windows.h>
#include <string>
#include <ostream>
#include <vector>

#include "ArgParser.h"

#include <iostream>

class wostream : std::wostream
{
public:
    typedef wchar_t char_type;
    typedef std::char_traits<wchar_t> traits_type;
    wostream(std::wostream& os) 
    : std::wostream(os.rdbuf()), stdOut(GetStdHandle( (&os == &std::wcout) ? STD_OUTPUT_HANDLE : STD_ERROR_HANDLE)),
        is_console(GetConsoleMode(stdOut, &numWritten) != 0)
    {}
    ~wostream(){}
    
    wostream& operator<<(const wchar_t& c)
    {
        flush();
        if (is_console)
        {
            WriteConsoleW(stdOut, &c, 1, &numWritten, NULL);
        }
        else
        {
            std::vector<char> buffer(WideCharToMultiByte(CP_UTF8, 0, &c, 1, NULL, 0, NULL, NULL));
            WideCharToMultiByte(CP_UTF8, 0, &c, 1, buffer.data(), (int)buffer.size(), NULL, NULL);
            buffer.emplace_back('\0');
            WriteFile(stdOut, buffer.data(), (DWORD)buffer.size() - 1, &numWritten, NULL);
        }
        return *this;
    }

    wostream& operator<<(const std::wstring& s)
    {
        flush();
        if (is_console)
        {
            WriteConsoleW(stdOut, s.c_str(), (DWORD)s.size(), &numWritten, NULL);
        }
        else
        {
            std::vector<char> buffer(WideCharToMultiByte(CP_UTF8, 0, s.c_str(), -1, NULL,0,  NULL, NULL));
            WideCharToMultiByte(CP_UTF8, 0, s.c_str(), -1, buffer.data(), (int)buffer.size(), NULL, NULL);
            WriteFile(stdOut, buffer.data(), (DWORD)buffer.size()-1, &numWritten, NULL);
        }
        return *this;
    }
    // considers 's' as utf-8 encoded string!
    wostream& operator<<(const std::string& s)
    {
        flush();
        if (is_console)
        {
            std::vector<wchar_t> buffer(MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, NULL,0 ));
            MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, buffer.data(), (int)buffer.size());
            WriteConsoleW(stdOut, buffer.data(), (DWORD)buffer.size()-1, &numWritten, NULL);
        }
        else
        {
            WriteFile(stdOut, s.data(), (DWORD)s.size()-1, &numWritten, NULL);
        }
        return *this;
    }
    template<typename Ty>
    wostream& operator<<(Ty t)
    {
        static_cast<std::wostream&>(*this) << t;
        return *this;
    }
    wostream& operator<<(std::wostream& (*f)(std::wostream&))
    {
        static_cast<std::wostream&>(*this) << f;
        return *this;
    }

private:
    const HANDLE stdOut;
    DWORD numWritten;
    const bool is_console;
};

static wostream wcout(std::wcout);
static wostream wcerr(std::wcerr);

int wmain(int argc, const wchar_t** argv)
{
    std::string filename;
    std::wstring wfilename;

    arg::Parser<std::wstring, true, wostream> parser(
        TEXT("A configurable application\n")
        TEXT("Author: Gabor Borbely, Contact: borbely@math.bme.hu\n")
        TEXT("Very long text will be sliced into 'width' long chunks. But a manual '\\n' will produce a new-line, too."),
        { TEXT("-h"), TEXT("--help") },
        wcout, wcerr,
        TEXT("ASCII\tabcde xyz\nGerman\täöü ÄÖÜ ß\n")
        TEXT("Hungarian\tőűíŐŰÍ\nPolish\tąęźżńłŁ\n")
        TEXT("Russian\tабвгдеж эюя\nCJK\t你好\nemoji\t😂"), 75);

    parser.AddArg(filename, { TEXT("-f"), TEXT("--file") }, TEXT("an ASCII filename"));
    parser.AddArg(wfilename, { TEXT("-w"), TEXT("--wide") }, TEXT("a Unicode filename"));

    parser.Do(argc, argv);

    return 0;
}
