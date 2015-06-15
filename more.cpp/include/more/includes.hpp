#ifndef _DEFS_HPP
#define _DEFS_HPP

// Architecture
#include "more\detail\x86.hpp"

// Compiler
#include "more\detail\msvc.hpp"

// Platform
#include "more\detail\windows.hpp"

// Library
#include "more\detail\stl.hpp"

// Other
//

// Defines
// Aligns to L1 cache.
#define L1CACHE_ALIGN MEM_ALIGN(CACHE_LINE_SIZE)

#endif