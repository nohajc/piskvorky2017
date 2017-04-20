//
// Created by nohajc on 19.4.17.
//
#ifdef _WIN32
#include <sciter-win-main.cpp>
#elif __APPLE__
#include <sciter-osx-main.cpp>
#elif __linux__
#include <sciter-gtk-main.cpp>
#else
#error "Unsupported platform"
#endif