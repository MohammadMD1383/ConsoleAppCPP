cmake_minimum_required(VERSION 3.17)
project(ConsoleAppCPP)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-bigobj)

add_executable(ConsoleAppCPP main.cpp commands.h util.h system_vars.h exprtk.hpp)

target_precompile_headers(${PROJECT_NAME} INTERFACE exprtk.hpp)
