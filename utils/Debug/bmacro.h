/* Copyright © 2023 Georgy E. All rights reserved. */

#ifndef __B_MACRO_H
#define __B_MACRO_H


#ifdef __cplusplus
extern "C" {
#endif


#include "bedug.h"


#ifndef BEDUG_ASSERT
#   define BEDUG_ASSERT(condition, message) if (!(condition)) { b_assert(__FILE__, __LINE__, message); }
#endif

#ifndef __STR_DEF__
#   define __STR_DEF__(x) #x
#endif

#ifndef __LOC__
#   define __LOC__ __FILE__ "(" __STR_DEF__(__LINE__) ")"
#endif

#ifndef _WARNING
#   define _WARNING(msgStr) message( __LOC__ " : PLEASE DON'T WORRY : " msgStr )
#endif


#ifdef __cplusplus
}
#endif


#endif
