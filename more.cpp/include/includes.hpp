#ifndef _DEFS_HPP
#define _DEFS_HPP

// Architecture
#include "x86.hpp"

// Compiler
#include "msvc.hpp"

// Platform
#include "windows.hpp"

// Library
#include "stl.hpp"

// Other
//

// Defines
// Aligns to L1 cache.
#define L1CACHE_ALIGN MEM_ALIGN(CACHE_LINE_SIZE)

#endif