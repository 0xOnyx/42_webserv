#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <istream>
# include <string>
# include <map>

class Request {
	private:
		std::string						_method;
		std::string						_uri;
		std::string						_protocol;
		map<std::string, std::string>	_header;

		void	parseRequestLine( std::string rLine );
	public:
		Request( int fd );
		~Request( void );


}

#endif