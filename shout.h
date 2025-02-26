#pragma once
#include <iostream>

#define okay(...) (std::cout << "[OKAY] " << __VA_ARGS__)
#define info(...) (std::cout << "[INFO] " << __VA_ARGS__)
#define warn(...) (std::cerr << "[WARN] " << __VA_ARGS__)

