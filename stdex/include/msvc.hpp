#ifndef WINDEFS_HPP
#define WINDEFS_HPP

#if defined _MSC_VER
#if defined _COMPILER_NAME
#error "Multiple compilers found?"
#else
#define COMPILER_NAME "MSVC"

// Version dependent defines
#if _MSC_VER > 1700
#define USE_STL_TYPES
#endif

// Import/Export symbols
#define EXPORT_SYMBOL __declspec(dllexport)
#define IMPORT_SYMBOL __declspec(dllimport)
#define HIDE_SYMBOL

// Linkage
#if defined _WINDLL
#define DYNAMIC_LINKAGE
#else
#define STATIC_LINKAGE
#endif

// Attributes
#define MEM_ALIGN(size) __declspec(align(size))

#endif//_COMPILER_NAME
#endif//_MSC_VER
#endif//WINDEFS_HPP