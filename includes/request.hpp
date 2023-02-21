#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "includes.h"

#include <iostream>
#include <sstream>
#include <map>
#include <cctype>
#include <cstdlib>
#include <vector>

# define CR '\r'
# define LF '\n'
# define CRLF "\r\n"

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
public:
    std::string	request_line[3];
    int			protocol[2];
    int         status;

    std::map<std::string, std::string>	_headers;
    std::vector<std::string>	_h_index;

    Request( std::string & buffer );
    ~Request( void );

    std::string	getHeaderValue( const std::string& key );
    std::map<std::string, std::string> &getHeaders();

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
private:
    static 	std::string	_valid_methods[];
    void	parseRequestLine( std::string rLine );
    bool	parseHeader( std::string header);
    bool	validMethod( );
    bool	validProtocol( );
    bool    tokenize( std::string & buffer, std::string & token, std::string delim );
};

#endif