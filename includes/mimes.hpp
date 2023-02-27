#ifndef MIMES_HPP
# define MIMES_HPP

class mimes
{
public:
	static const char	*get_type(const char *path);
private:
	typedef struct entry {
		const char *file_extension;
		const char *mime_type;
	} t_entry;
	static mimes::t_entry types[1200];
	static int	strcmpi(const char *s1, const char *s2);
};

#endif