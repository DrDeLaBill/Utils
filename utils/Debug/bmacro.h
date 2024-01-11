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

#ifndef __STR_DEF2__
#   define __STR_DEF2__(x) #x
#endif

#ifndef __STR_DEF1__
#   define __STR_DEF1__(x) __STR_DEF2__(x)
#endif

#ifndef __LOC__
#   define __LOC__ __FILE__ "(" __STR_DEF1__(__LINE__) ")"
#endif

#ifndef _WARNING
#   define _WARNING(msgStr) message( __LOC__ " : an unimportant warning has been occurred, PLEASE DON'T WORRY, keep using the system : " msgStr )
#endif


#ifdef __cplusplus
}
#endif


#endif
