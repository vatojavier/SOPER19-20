#ifndef TYPES_H
#define TYPES_H

#include <limits.h>

#define ERROR_INT INT_MIN

#define MAX_STATES 20
#define ERROR_STATE (-1)
#define UNINITIALIZED_STATE (-2)

#define DEBUG 1

typedef enum BOOL {
	TRUE = 1,
	FALSE = 0
} BOOL;

typedef enum STATUS {
	ERROR = -1,
	SUCCESS
} STATUS;

#endif //TYPES_H