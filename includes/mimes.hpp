#ifndef MIMES_HPP
# define MIMES_HPP
# include "includes.h"

class mimes
{
public:
	static std::string	get_type(const char *path);
private:
	typedef struct entry {
		const char *file_extension;
		const char *mime_type;
	} t_entry;
	static mimes::t_entry types[1002];
	static int	strcmpi(const char *s1, const char *s2);
};

#endif