#ifndef CUSTOM_TYPES_H
#define CUSTOM_TYPES_H

typedef signed char i8;
typedef signed short i16;
typedef signed int i32;
typedef signed long i64;
typedef signed long long isize;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long u64;
typedef unsigned long long usize;

typedef enum {
  false = 0,
  true = 1,
} bool;

#include "./string.h"

#endif
