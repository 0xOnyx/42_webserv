#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "includes.h"

# define CRLF "\r\n"

enum e_status_codes {
    CONTINUE = 100,
    OK = 200,
    CREATED = 201,
    ACCEPTED = 202,
    NO_CONTENT = 204,
    MULTIPLE = 300,
    BAD_REQUEST = 400,
    UNAUTHORIZED = 401,
    FORBIDDEN = 403,
    NOT_FOUND = 404,
    METHOD_NOT_ALLOWED = 405,
    LENGTH_REQUIRED = 411,
    PAYLOAD = 413,
    SERVER_ERROR = 500,
    NOT_IMPLEMENTED = 501,
    HTTP_VERSION = 505
};


class Response {
private:
    static std::map<int, std::string>   _status_codes;
    static bool                         _initialized;
    int                                 _status;
    size_t                              _len;
    std::string                         _type;
    std::string                         _body;
    std::string                         _response;
    static void initStatusCode( void );

    const std::string getStatusLine( void );
    const std::string getGeneralHeader( void );
    const std::string getResponseHeader( void );
    const std::string getEntityHeader( void );
    std::string setDate( void );
public:
    Response( int status, size_t b_len, std::string b_type, std::string body);
    Response( int status );
    ~Response( void );

    const std::string&  getResponse( void );
};

#endif

