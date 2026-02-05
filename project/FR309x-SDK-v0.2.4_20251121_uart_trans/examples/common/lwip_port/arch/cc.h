#ifndef __CC_H
#define __CC_H

#include "cmsis_compiler.h"

#ifdef __CC_ARM
#define PACK_STRUCT_STRUCT                  __attribute__((packed))
#elif defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6100100)
#define PACK_STRUCT_STRUCT                  __attribute__((packed, aligned(1)))
#elif defined (__GNUC__)
#define PACK_STRUCT_STRUCT                  __attribute__((packed))
#else
#error("unsupported compiler.\r\n")
#endif

#endif  // __CC_H
