#include "includes.h"

size_t	get_size_file(int fd)
{
	struct stat	sb;

	if (fstat(fd, &sb) < 0)
		throw std::runtime_error("error to get size of file");
	return (sb.st_size);
}

std::string string_to_lower( std::string str ) {
    std::string result;
    size_t len = str.size();

    for ( size_t i = 0; i < len; i++ ) {
        result += static_cast<char>(std::tolower(str[i]));
    }
    return result;
}