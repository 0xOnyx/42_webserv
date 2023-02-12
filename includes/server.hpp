#ifndef SERVER_HPP
# define SERVER_HPP
# include "includes.h"

class Engine;

class Server
{
public:
	Server();
	void		add_engine(std::string path, class *Engine);
	void		request(t_list *first);
	const char 	*get_servername(void) const;
	~Server();
private:
	char											*servername;
	std::map<const char *, class *Engine, cmp_str>	location;
};

#endif