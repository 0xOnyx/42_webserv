#include "includes.h"

const char* CGIParser::ParseError::what() const throw( ){
    return "CGI Parse Error";
}

CGIParser::CGIParser( const std::string & buffer ) {
    std::istringstream  ss(buffer);
    std::string         token;

    while (std::getline(ss, token, '\n') && !token.empty()) {
        size_t      delim = token.find(": ");
        if (delim == std::string::npos)
            throw ParseError();
        std::string key = token.substr(0, delim);

        token.erase(0, delim + 2);
        if (token.empty())
            throw ParseError();
        _headers[key] = token;
    }
    while (std::getline(ss, token)) {
        _body.append(token);
        _body.append("\n");
    }
    if (_body.empty())
        throw ParseError();
}

CGIParser::~CGIParser( ) {}

const std::map<std::string, std::string>&   CGIParser::getHeaders( ) {
    return (this->_headers);
}

const std::string&  CGIParser::getBody( ) {
    return (this->_body);
}
