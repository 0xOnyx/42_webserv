#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "includes.h"
class Request {
	private:
		std::string						_method;
		std::string						_uri;
		std::string						_protocol;
		map<std::string, std::string>	_header;

		void	parseRequestLine( std::string rLine );
		Request( int fd );
	public:

		~Request( void );


}

#endif