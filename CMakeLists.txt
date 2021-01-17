cmake_minimum_required(VERSION 3.17)
project(ConsoleAppCPP)

set(CMAKE_CXX_STANDARD 20)

add_executable(ConsoleAppCPP main.cpp commands.h util.h system_vars.h)