/*
Amju Games source code (c) Copyright Juliet Colman 2004
*/

#pragma once

#include <algorithm>
#include <bit>      // For std::endian - C++20
#include <cstring>   // For std::memcpy

#include "AmjuTypes.h"

// Swap byte order if neccessary. 
namespace Amju
{
template <typename T>
T Endian(T val) 
{
    if constexpr (std::endian::native == std::endian::big) 
    {
        // 1. Copy the object into a byte array safely
        unsigned char bytes[sizeof(T)];
        std::memcpy(bytes, &val, sizeof(T));
        
        // 2. Reverse the bytes in place
        std::reverse(bytes, bytes + sizeof(T));
        
        // 3. Copy the reversed bytes back into a temporary result
        T result;
        std::memcpy(&result, bytes, sizeof(T));
        return result;
    }
    
    // On Little Endian, this is all that runs
    return val;
}
}

