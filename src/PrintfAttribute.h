#ifndef GNILK_PRINTF_ATTRIBUTE_H
#define GNILK_PRINTF_ATTRIBUTE_H

// __attribute__((format(printf()))) works a bit differently in C++ than in C.
// If you just pass the actual argument number of the format string, gcc will
// refuse to compile because the first argument actually refers to the implicit
// "this" argument and not the actual first argument to the function.
// This means the same macro won't compile in both C and C++. Increase the
// index by 1 here when compiling C++, but not C, since having any index other
// than the actual argument index is confusing.
#if defined (__GNUC__)
#ifdef __cplusplus
#define GNILK_PRINTF_ATTRIB(formatidx) __attribute__((format(printf, (formatidx) + 1, (formatidx) + 2)))
#else
#define GNILK_PRINTF_ATTRIB(formatidx) __attribute__((format(printf, (formatidx), (formatidx) + 1)))
#endif // __cplusplus
#else
#define GNILK_PRINTF_ATTRIB(formatidx)
#endif

#endif //
