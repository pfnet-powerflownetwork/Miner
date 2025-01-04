/*
* Copyright 2023 Devi991
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the Free
* Software Foundation; either version 2 of the License, or (at your option)
* any later version.
*/

#ifndef PFNETPLFUNCTIONS_H_
#define PFNETPLFUNCTIONS_H_

#include <stdint.h>

#ifndef MAX
	#define MAX(a,b) ((a) > (b) ? a : b)
	#define MIN(a,b) ((a) < (b) ? a : b)
#endif

extern int32_t gcd(int32_t	a, int32_t b);

#endif // PFNETPLFUNCTIONS_H_
