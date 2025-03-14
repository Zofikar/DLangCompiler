#pragma once

#include <cassert>

# if defined _MSC_VER
#define UNREACHABLE __assume(0)
#else
#define UNREACHABLE __builtin_unreachable()
#endif

#ifdef NDEBUG

# if defined __cplusplus
#  define assert_assume_message(expr, message)							\
(static_cast <bool> (expr)						\
? UNREACHABLE							\
: void (0))
# elif !defined __GNUC__ || defined __STRICT_ANSI__
#  define assert_assume_message(expr, message)							\
((expr)								\
? UNREACHABLE						\
: __ASSERT_VOID_CAST (0))
# else
/* The first occurrence of EXPR is not evaluated due to the sizeof,
   but will trigger any pedantic warnings masked by the __extension__
   for the second occurrence.  The ternary operator is required to
   support function pointers and bit fields in this context, and to
   suppress the evaluation of variable length arrays.  */
#  define assert_assume_message(expr, message)							\
((void) sizeof ((expr) ? 1 : 0), __extension__ ({			\
if (expr)								\
UNREACHABLE;							\
else								\
; /* empty */	\
}))
# endif


#else // NODEBUG

# if defined __cplusplus
#  define assert_assume_message(expr, message)							\
(static_cast <bool> (expr)						\
? void (0)							\
: __assert_fail (message, __FILE__, __LINE__, __ASSERT_FUNCTION))
# elif !defined __GNUC__ || defined __STRICT_ANSI__
#  define assert_assume_message(expr, message)							\
((expr)								\
? __ASSERT_VOID_CAST (0)						\
: __assert_fail (message, __FILE__, __LINE__, __ASSERT_FUNCTION))
# else
/* The first occurrence of EXPR is not evaluated due to the sizeof,
   but will trigger any pedantic warnings masked by the __extension__
   for the second occurrence.  The ternary operator is required to
   support function pointers and bit fields in this context, and to
   suppress the evaluation of variable length arrays.  */
#  define assert_assume_message(expr, message)							\
((void) sizeof ((expr) ? 1 : 0), __extension__ ({			\
if (expr)								\
; /* empty */							\
else								\
__assert_fail (message, __FILE__, __LINE__, __ASSERT_FUNCTION);	\
}))
# endif

#endif // NODEBUG


#define assert_assume(expr) assert_assume_message(expr, #expr)