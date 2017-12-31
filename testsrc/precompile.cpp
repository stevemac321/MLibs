#include "precompile.hpp"
void STEVEMAC_TRACE(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
}

int passed=0;
int failed=0;
int tcs=0;
