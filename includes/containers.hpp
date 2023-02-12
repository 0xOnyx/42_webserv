#ifndef CONTAINERS_HPP
# define CONTAINERS_HPP
# include "includes.h"

class Socket;

typedef std::pair<std::string, std::string> pair_str;

class containers
{
public:

private:
	std::map<pair_str, class *Socket>	_socket;
};

#endifx