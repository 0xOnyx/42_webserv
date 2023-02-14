#include "includes.h"

size_t	get_size_file(int fd)
{
	struct stat	sb;

	if (fstat(fd, &sb) < 0)
		throw std::runtime_error("error to get size of file");
	return (sb.st_size);
}