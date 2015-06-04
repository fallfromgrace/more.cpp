#ifndef _X86DEFS_HPP
#define _X86DEFS_HPP

#define _ARCHITECTURE_NAME "x86"

#if defined _M_IX86 || _M_X64
#define PLATFORM_X86
#define CACHE_LINE_SIZE 64
#endif

#endif//_X86DEFS_HPP