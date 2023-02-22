#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "data.h"

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
	int			socketfd;
    std::string	request_line[3];
    int			protocol[2];
    Components  _URI[5];
    std::string _body;
    size_t      content_length;

    std::map<std::string, std::string>	_headers;
    std::vector<std::string>	_h_index;

    Request( int _sockfd, std::string & buffer );
    ~Request( void );

    std::string	getHeaderValue( const std::string& key );
    std::map<std::string, std::string> &getHeaders();
    std::string getURI( void );
    std::string getURIComp( int component );

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
	size_t  has_body( void );
	void    set_body( std::string body );
private:
    static 	std::string	_valid_methods[];

    void	parseRequestLine( std::string rLine );
    void	parseHeader( std::string header);
    bool    parseURI( std::string & buffer );
    bool	validMethod( );
    bool	validProtocol( );
    bool    tokenize( std::string & buffer, std::string & token, std::string delim );
    int     tokenizeURI( std::string & buffer, std::string & token, std::string array, bool trim);
};

#endif