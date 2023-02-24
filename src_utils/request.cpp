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

Request::Request( int _sockfd, std::string & buffer ) : socketfd(_sockfd)
{
    std::string token;

    tokenize(buffer, token, CRLF);
    this->parseRequestLine(token);

    while(tokenize(buffer, token, CRLF) and token.size()) {
        this->parseHeader(token); }

    this->parseURI(this->request_line[URI]);
}

Request::~Request() {}

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

    if (!std::getline(ss, token, '/') && token != "HTTP")
        return false;
    if (!std::getline(ss, token, '.') && !is_a_number(token))
        return false;
    this->protocol[MAJOR] = std::atoi(token.data());
    if (!std::getline(ss, token, '.') && !is_a_number(token))
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

    if (!header.compare(0, 1, " ") || !header.compare(0, 1, "	")) {
        throw std::runtime_error("bad request");
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

bool    Request::tokenize( std::string & buffer, std::string & token, std::string delim ) {
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

int    Request::tokenizeURI( std::string & buffer, std::string & token, std::string array, bool trim ) {
    size_t pos = buffer.find_first_of(array);
    size_t delim = 0;
    char ret = 0;

    token.clear();
    if (trim)
        delim = 1;
    if ( pos != std::string::npos ) {
        ret = buffer[pos];
        token = buffer.substr(0, pos);
        buffer.erase(0, pos + delim);

    } else {
        token = buffer;
        buffer.clear();
    }
    switch (ret) {
        case 47:
            return PATH;
        case 63:
            return QUERY;
        case 35:
            return FRAG;
        default:
            return END_URI;
    }
}

bool    Request::parseURI( std::string & buffer ) {
    std::string tmp = buffer;
    std::string token;
    int         hierarchy;

    for (int i = SCHEME; i < END_URI; i++) {
        this->_URI[i].first = false;
    }

    if (std::isalpha(tmp[0])) {
        hierarchy = SCHEME;
    } else if (tmp.compare(0, 2, "//") == 0) {
        hierarchy = AUTH;
    } else if (tmp[0] == '/') {
        hierarchy = PATH;
    } else {
        return false;
    }
    while (hierarchy < END_URI) {
        switch (hierarchy) {
            case SCHEME:
                tokenizeURI(tmp, token, ":", true);
                token.append(":");
                if (tmp.compare(0, 2, "//") == 0) {
                    hierarchy = AUTH;
                } else {
                    hierarchy = PATH;
                }
                this->_URI[SCHEME].first = true;
                this->_URI[SCHEME].second = token;
                break;
            case AUTH:
                tmp.erase(0, 2);
                hierarchy = tokenizeURI(tmp, token, "/?#", false);
                token.insert(0, "//");
                this->_URI[AUTH].first = true;
                this->_URI[AUTH].second = token;
                break;
            case PATH:
                hierarchy = tokenizeURI(tmp, token, "?#", false);
                this->_URI[PATH].first = true;
                this->_URI[PATH].second = token;
                break;
            case QUERY:
                hierarchy = tokenizeURI(tmp, token, "#", false);
                this->_URI[QUERY].first = true;
                this->_URI[QUERY].second = token;
                break;
            case FRAG:
                this->_URI[FRAG].first = true;
                this->_URI[FRAG].second = tmp;
                hierarchy = END_URI;
                break;
        }
    }
    if (!this->_URI[SCHEME].first) {
        tmp = this->request_line[PROTOCOL];
        tokenizeURI(tmp, token, "/", true);
        token.append(":");
        this->_URI[SCHEME].first = true;
        this->_URI[SCHEME].second = string_to_lower(token);
    }

    if (!this->_URI[AUTH].first) {
        token = getHeaderValue("Host");
        if (token.size()) {
            token.insert(0, "//");
            this->_URI[AUTH].first = true;
            this->_URI[AUTH].second = token;
        }
    }
    return true;
}

std::string Request::getURI( void ) {
    std::string result("");

    for (int i = SCHEME; i < END_URI; i++) {
        if (this->_URI[i].first)
            result.append(this->_URI[i].second);
    }
    return result;
}

std::string Request::getURIComp( int component ) {
    std::string result;

    result.clear();
    if (component < END_URI and this->_URI[component].first) {
        result = this->_URI[component].second;
    }

    return result;
}

size_t  Request::has_body( void ) {
	if (_headers.find("Content-Length") != _headers.end()) {
		content_length = std::atoi(_headers["Content-Length"].data());
	}
	return content_length;
}


void	Request::set_body(std::string body)
{
	_body = body;
}

std::string &Request::get_body(void)
{
	return (_body);
}