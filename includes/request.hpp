#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "data.h"

# define CR '\r'
# define LF '\n'
# define CRLF "\r\n"

enum e_methods {
    OPTIONS,
    GET,
    HEAD,
    POST,
    PUT,
    DELETE,
    TRACE,
    CONNECT,
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
	size_t      						content_length;
	std::string 						_body;
	std::string							request_line[3];
	int									protocol[2];
	int         						socketfd;
	Components  						_URI[5];
	std::map<std::string, std::string>	_headers;
    std::vector<std::string>			_h_index;
	size_t  has_body( void );

    Request( int _sockfd, std::string & buffer );
    ~Request( void );

    std::string	getHeaderValue( const std::string& key );
    std::map<std::string, std::string> &getHeaders();
    std::string getURI( void );
    std::string getURIComp( int component );

	std::string	&get_body(void);
	void		set_body(std::string body);

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
    bool    parseURI( std::string & buffer );
    bool	validMethod( );
    bool	validProtocol( );
    bool    tokenize( std::string & buffer, std::string & token, std::string delim );
    int     tokenizeURI( std::string & buffer, std::string & token, std::string array, bool trim);
};

#endif