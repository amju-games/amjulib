// * AMJULIB *
// (c) Copyright Juliet Colman 2000-2026
#pragma once

#ifdef WIN32
#define _USE_MATH_DEFINES
#define NOMINMAX             // Prevents min/max macro conflicts with std::min/max
#define _WINSOCKAPI_   // Prevents windows.h from loading winsock.h
#include <WinSock2.h> 
#include <Windows.h>
#endif

#include <algorithm>
#include <array>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>
#include <vector>
