#ifndef STDARG_H
#define STDARG_H

typedef char* va_list;

#define _va_round_size(TYPE) \
	((sizeof(TYPE)+sizeof(int) - 1) / sizeof(int))* sizeof(int)
/*
 *the size must be the multiple of sizeof(int),that all...
*/

#define va_start(AP,PTR) \
	(AP = (char *)&(PTR) + _va_round_size(PTR))

#define va_arg(AP,TYPE) \
	(AP += _va_round_size(TYPE),*(TYPE*)(AP - _va_round_size(TYPE)))

#define va_end(AP) (AP = (va_list)0)

#endif
