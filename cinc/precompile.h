/*==============================================================================
 Name        : precompile.h
 Author      : Stephen MacKenzie
 Copyright   : Licensed under GPL version 2 (GPLv2)
==============================================================================*/
#pragma once
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>

#ifndef _countof
#define _countof(x) sizeof(x) / sizeof(x[0])
#endif

#ifndef _gridcountof
#define _gridcountof(x) sizeof(x) / sizeof(x[0][0])
#endif

typedef void *genptr;



