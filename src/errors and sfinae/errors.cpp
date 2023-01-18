#include "errors.h"

void err::exit_if(bool cnd, const char* msg)
{
	if (cnd)
	{
		std::cerr << msg << std::endl;
		abort();
	}
}

