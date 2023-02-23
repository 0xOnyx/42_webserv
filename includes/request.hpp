#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "data.h"
# include <iostream>
# include <sstream>
# include <map>
# include <cctype>
# include <cstdlib>
# include <vector>
# include <utility>

# define CR '\r'
# define LF '\n'
# define CRLF "\r\n"

# define CONTINUE 100
# define OK 200
# define BAD_REQUEST 400


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

enum uri_hierarchy {
    SCHEME,
    AUTH,
    PATH,
    QUERY,
    FRAG,
    END_URI
};

typedef std::pair<bool,std::string> Components;

class Request {
public:
    std::string	request_line[3];
    int			protocol[2];
    int         status;
    Components  URI_comp[5];
    std::map<std::string, std::string>	_headers;
    std::vector<std::string>	_h_index;

    Request( std::string & buffer );
    ~Request( void );

    std::string	getHeaderValue( const std::string& key );
    std::map<std::string, std::string> &getHeaders();
    std::string getURI( void );
    std::string getURIComp( int component );

    class BadRequest: public std::exception {
        virtual const char* what() const throw();
    };

private:
    static 	std::string	_valid_methods[];

    void    handleStatusCode( void );

    void	parseRequestLine( std::string rLine );
    bool	parseHeader( std::string header);
    bool    parseURI( std::string & buffer );

    bool    tokenize( std::string & buffer, std::string & token, std::string delim );
    int     tokenizeURI( std::string & buffer, std::string & token, std::string array, bool trim);

    bool	validMethod( );
    bool	validProtocol( );
    bool    validSCHEME( const std::string & token );
};

#endif