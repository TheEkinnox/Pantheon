#pragma once

// The following macros provide a better alternative to `#if SYMBOL` and should be used in its stead in the code base.
// The main benefit being it emits a compiler error when the tested symbol is not defined instead of silently defaulting to false
// - If `MY_SYMBOL` is defined to `IN_USE` or `USE_IF( <true_expression> )` then `USING( MY_SYMBOL )` will evaluate to true.
// - If `MY_SYMBOL` is defined to `NOT_IN_USE` or `USE_IF( <false_expression> )` then `USING( MY_SYMBOL )` will evaluate to false.
// - Otherwise, if `MY_SYMBOL` is undefined or defined to an invalid value, `USING( MY_SYMBOL )` will emit a compiler error

#define IN_USE ==
#define NOT_IN_USE !=

#define USE_IF(x) && ((x) ? 1 : 0) &&

#define USING(x) (1 x 1)
