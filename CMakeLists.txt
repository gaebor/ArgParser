cmake_minimum_required (VERSION 3.1)
project (argparser)

set (CMAKE_CXX_STANDARD 11)

if (WIN32)
    add_compile_options( /DUNICODE /D_UNICODE /utf-8 )
    add_executable(wmain wmain.cpp ArgParser.h)
endif()

add_executable(main main.cpp ArgParser.h)
