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
    MOVED = 301,
    FOUND = 302,
    OTHER = 303,
    NOT_MOD = 304,
    TMP_REDIR = 307,
    PERM_REDIR = 308,
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
    std::string                         _location;
	std::string 						_allowed;
    static void initStatusCode( void );

    const std::string getStatusLine( void );
    std::string getGeneralHeader( void );
    std::string getResponseHeader( void );
    const std::string getEntityHeader( void );
    std::string setDate( void );
public:
    Response(int status, size_t b_len, std::string b_type, std::string body);
	Response(int status, size_t b_len, std::string b_type, std::string body, std::string data);
    Response(int status, size_t body_length, std::string body_type, std::string body, std::map<std::string, std::string> header_values);
    Response( int status, std::string data);
    Response( int status );
    ~Response(void );

    const std::string&  getResponse( void );
};

#endif

