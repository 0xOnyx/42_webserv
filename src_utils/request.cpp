#include "includes.h"

std::string	Request::_valid_methods[] = {"GET", "POST", "DELETE"};

bool is_a_number(const std::string& s) {

    for (std::string::const_iterator it = s.begin(); it != s.end(); ++it) {
        if (!isdigit(*it)) {
            return false;
        }
    }
    return true;
}

Request::Request( std::string & buffer ) : status(200), content_length(0) {
    std::string token;

    _body.clear();
    tokenize(buffer, token, CRLF);
    this->parseRequestLine(token);

    while(tokenize(buffer, token, CRLF) and token.size()) {
        this->parseHeader(token); }
}

Request::~Request( void ) {}

const char* Request::InvalidRequestLine::what() const throw( ){
    return "Invalid Request Line";
}

const char* Request::InvalidMethod::what() const throw( ){
    return "Invalid HTTP Method";
}

const char* Request::InvalidURI::what() const throw( ){
    return "Invalid URI";
}

const char* Request::InvalidProtocol::what() const throw( ){
    return "Invalid Protocol";
}

bool	Request::validMethod( ) {
    for (int i = 0; i < NUM_METHODS; i++) {
        if (request_line[METHOD] == Request::_valid_methods[i])
            return true;
    }
    return false;
}

bool	Request::validProtocol( void ) {
    std::istringstream ss(this->request_line[PROTOCOL]);
    std::string token;

    if (!std::getline(ss, token, '/') and token != "HTTP")
        return false;
    if (!std::getline(ss, token, '.') and !is_a_number(token))
        return false;
    this->protocol[MAJOR] = std::atoi(token.data());
    if (!std::getline(ss, token, '.') and !is_a_number(token))
        return false;
    this->protocol[MINOR] = std::atoi(token.data());
    return true;
}

void	Request::parseRequestLine( std::string rLine ) {
    std::istringstream ss(rLine);
    std::string token;

    for (int i = METHOD; i <= PROTOCOL; i++) {
        if (std::getline(ss, token, ' '))
            this->request_line[i] = token;
        else
            throw Request::InvalidRequestLine();
    }
    if (std::getline (ss, token, ' '))
        throw Request::InvalidRequestLine();
    if (!this->validMethod())
        throw Request::InvalidMethod();
    if (!this->validProtocol())
        throw Request::InvalidProtocol();
}

bool	Request::parseHeader( std::string header ) {
    std::string key;

    if (!header.compare(0, 1, " ") or !header.compare(0, 1, "	")) {
        this->status = 400;
        return false;
    } else {
        tokenize(header, key, ": ");
        this->_h_index.push_back(key);
        this->_headers[key] = header;
        return true;
    }
}

std::string	Request::getHeaderValue( const std::string& key ) {
    std::map<std::string, std::string>::iterator it;

    it = _headers.find(key);
    if (it != this->_headers.end())
        return it->second;
    else
        return "";
}

std::map<std::string, std::string> &    Request::getHeaders() {
    return this->_headers;
}

bool Request::tokenize( std::string & buffer, std::string & token, std::string delim ) {
    size_t pos = buffer.find(delim);
    if ( pos != std::string::npos ) {
        token.clear();
        token = buffer.substr(0, pos);
        buffer.erase(0, pos + delim.size());
        return true;
    } else {
        return false;
    }
}

size_t  Request::has_body( void ) {
    if (_headers.find("Content-Length") != _headers.end()) {
        content_length = std::atoi(_headers["Content-Length"].data());
    }
    return content_length;
}

void    Request::set_body( std::string & body ) {
    _body = body;
}