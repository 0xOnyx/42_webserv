#ifndef CGI_PARSER
# define CGI_PARSER

# include "data.h"

class CGIParser {
private:
    std::map<std::string, std::string>   _headers;
    std::string                             _body;

    class ParseError: public std::exception {
        virtual const char* what() const throw();
    };

public:
    CGIParser(const std::string & buffer);
    ~CGIParser( void );

    const std::map<std::string, std::string>&   getHeaders( void );
    const std::string&                          getBody( void );
};

#endif