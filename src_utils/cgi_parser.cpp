#include "includes.h"

const char* CGIParser::ParseError::what() const throw( ){
    return "CGI Parse Error";
}

CGIParser::CGIParser( const std::string & buffer ) {
    std::istringstream  ss(buffer);
    std::string         token;

    while (std::getline(ss, token, '\n') && token.size()) {
        size_t      delim = token.find(": ");
        if (delim == std::string::npos)
            throw ParseError();
        std::string key = token.substr(0, delim);

        token.erase(0, delim + 2);
        if (!token.size())
            throw ParseError();
        _headers[key] = token;
    }
    while (std::getline(ss, token)) {
        _body.append(token);
        _body.append("\n");
    }
    if (!_body.size())
        throw ParseError();
}

CGIParser::~CGIParser( void ) {}

const std::map<std::string, std::string>&   CGIParser::getHeaders( void ) {
    return (this->_headers);
}

const std::string&  CGIParser::getBody( void ) {
    return (this->_body);
}
