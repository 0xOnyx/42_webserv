#include "includes.h"


struct cmp_str
{
	bool operator()(char const *a, char const *b) const
	{
		return (strcmp(a, b) != 0);
	}
};