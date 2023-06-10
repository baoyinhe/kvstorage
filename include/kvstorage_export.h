#pragma once

#ifdef KVSTORAGE_SHARED_BUILD
#ifdef _WIN32
#ifdef KVSTORAGE_EXPORTS
#define KVSTORAGE_API __declspec(dllexport)
#else
#define KVSTORAGE_API __declspec(dllimport)
#endif  // KVSTORAGE_EXPORTS
#else
#define KVSTORAGE_API
#endif  // _WIN32
#else
#define KVSTORAGE_API
#endif  // KVSTORAGE_SHARED_BUILD
