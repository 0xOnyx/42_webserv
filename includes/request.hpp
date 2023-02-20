#ifndef REQUEST_HPP
# define REQUEST_HPP

#include <iostream>
#include <sstream>
#include <map>
#include <cctype>
#include <cstdlib>
#include <vector>

enum e_methods {
	GET,
	POST,
	DELETE,
	NUM_METHODS
};

enum {
	METHOD,
	URI,
	PROTOCOL
};

enum {
	MAJOR,
	MINOR
};

class Request {
	private:
		static std::string	_valid_methods[];


		void	parseRequestLine( std::string rLine );
		void	parseHeader( std::string header);
		bool	validMethod( void );
		bool	validProtocol( void );
	public:
		std::string	request_line[3];
		int			protocol[2];
		std::map<std::string, std::string>	headers;
		std::vector<std::string>	_h_index;

		Request(const std::string & buffer );
		~Request( void );

		const std::string	getHeaderValue( const std::string& key );
		const std::map<std::string, std::string>	getHeaders( void );

		class InvalidRequestLine: public std::exception {
			virtual const char* what() const throw();
		};

		class InvalidMethod: public std::exception {
			virtual const char* what() const throw();
		};

		class InvalidURI: public std::exception {
			virtual const char* what() const throw();
		};

		class InvalidProtocol: public std::exception {
			virtual const char* what() const throw();
		};

};

#endif