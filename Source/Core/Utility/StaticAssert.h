#pragma once
#include <kvs/Compiler>
#include <iostream>

#if defined KVS_COMPILER_GCC
  #if !defined(__cpp_static_assert) && !defined(static_assert)
    #if KVS_COMPILER_VERSION_LESS_THAN( 3, 1 )
      #define KVS_STATIC_ASSERT( expr, mes ) typedef char KVS_STATIC_ASSERTION_FAILURE[ ( expr ) ? 1 : -1 ]
    #else
      #define KVS_STATIC_ASSERT( expr, mes ) typedef char KVS_STATIC_ASSERTION_FAILURE[ ( expr ) ? 1 : -1 ] __attribute__((unused))
    #endif
  #else
    #if defined(__GNUC__) && ((__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7))) || (defined(__apple_build_version__) && (__apple_build_version__ >= 7000000))
      // #define KVS_STATIC_ASSERT( expr, mes ) static_assert( expr, mes ) __attribute__((unused))
      #define KVS_STATIC_ASSERT( expr, mes ) static_assert( expr, mes )
    #else
      #define KVS_STATIC_ASSERT( expr, mes ) static_assert( expr, mes )
    #endif
  #endif
#elif defined KVS_COMPILER_VC
  #if KVS_COMPILER_VERSION_LESS_THAN( 10, 0 )
    #define KVS_STATIC_ASSERT( expr, mes ) typedef char KVS_STATIC_ASSERTION_FAILURE[ ( expr ) ? 1 : -1 ]
  #else
    #define KVS_STATIC_ASSERT( expr, mes ) static_assert( expr, mes )
  #endif
#elif defined KVS_COMPILER_FCC
  #define KVS_STATIC_ASSERT( expr, mes ) typedef char KVS_STATIC_ASSERTION_FAILURE[ ( expr ) ? 1 : -1 ]
#else
  #error "Not Supported Compiler"
#endif