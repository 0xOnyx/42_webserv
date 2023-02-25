#include "includes.h"


#define GENDELIMS ":/?#[]@"
#define SUBDELIMS "!$&'()*+,;="

std::string	Request::_valid_methods[] = { "OPTIONS", "GET", "HEAD", "POST", "PUT", "DELETE", "TRACE", "CONNECT"};

bool    is_a_number(const std::string& s) {

    for (std::string::const_iterator it = s.begin(); it != s.end(); ++it) {
        if (!isdigit(*it)) {
            return false;
        }
    }
    return true;
}

bool    is_unreserved_char(const char c) {
    if (!isalnum(c) && c != '-' && c != '.' && c != '_' && c != '~') {
        return false; }
    return true;
}

bool    is_gen_delims(const char c) {
    std::string genD(GENDELIMS);
    size_t pos = genD.find(c);

    if ( pos == std::string::npos) {
        return false; }
    return true;
}

bool    is_sub_delims(const char c) {
    std::string subD(SUBDELIMS);
    size_t pos = subD.find(c);

    if ( pos == std::string::npos) {
        return false; }
    return true;
}

bool    is_reserved(const char c) {
    if ( is_sub_delims(c) or is_gen_delims(c) )
        return true;
    return false;
}

Request::Request( int _sockfd, std::string & buffer ) : socketfd(_sockfd), status(CONTINUE) {
    std::string token;

    _body.clear();
    if (!tokenize(buffer, token, CRLF)) {
        throw Request::BadRequest(); }
    parseRequestLine(token);
    while (tokenize(buffer, token, CRLF) && token.size()) {
        if (!parseHeader(token)) {
            throw Request::BadRequest(); }
    }
    parseURI(this->request_line[URI]);
    status = OK;
}

Request::~Request( void ) {}

// **************************************************************************** //
//							    EXCEPTION									    //
// **************************************************************************** //

void    Request::handleStatusCode() {
    switch (status) {
        case BAD_REQUEST:
            throw Request::BadRequest();
        default:
            break;
    }
}

const char* Request::BadRequest::what() const throw( ){
    return "400 Bad Request";
}

// **************************************************************************** //
//							        BODY									    //
// **************************************************************************** //

size_t  Request::has_body( void ) {
    if (_headers.find("Content-Length") != _headers.end()) {
        content_length = std::atoi(_headers["Content-Length"].data());
    }
    return content_length;
}

void    Request::set_body( std::string body ) {
    _body = body;
}

std::string&    Request::get_body()
{
    return (_body);
}

// **************************************************************************** //
//							        GETTERS									    //
// **************************************************************************** //

std::string	Request::getHeaderValue( const std::string& key ) {
    std::map<std::string, std::string>::iterator it;

    if (status != OK) {
        handleStatusCode(); }
    it = _headers.find(key);
    if (it != this->_headers.end())
        return it->second;
    else
        return "";
}

std::map<std::string, std::string> &    Request::getHeaders() {
    if (status != OK) {
        handleStatusCode(); }
    return this->_headers;
}

std::string Request::getURIComp( int component ) {
    std::string result("");

    if (status != OK) {
        handleStatusCode(); }
    if (component < END_URI && this->URI_comp[component].first) {
        result = this->URI_comp[component].second;
    }

    return result;
}

// **************************************************************************** //
//							        PARSING									    //
// **************************************************************************** //


void	Request::parseRequestLine( std::string rLine ) {
    std::istringstream ss(rLine);
    std::string token;

    for (int i = METHOD; i <= PROTOCOL; i++) {
        if (std::getline(ss, token, ' '))
            this->request_line[i] = token;
        else
            throw Request::BadRequest();
    }
    if (std::getline (ss, token, ' ') || !validMethod() || !validProtocol())
        throw Request::BadRequest();
}

bool	Request::parseHeader( std::string header ) {
    std::string key;

    if (!header.compare(0, 1, " ") || !header.compare(0, 1, "	")) {
        std::string last_key = *this->_h_index.rbegin();
        while (!header.compare(0, 1, " ") || !header.compare(0, 1, "	")) {
            header.erase(0, 1); }
        this->_headers[last_key].append(header);
    } else {
        tokenize(header, key, ": ");
        this->_h_index.push_back(key);
        this->_headers[key] = header;
        return true;
    }
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
        this->URI_comp[i].first = false;
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
                if (tmp.compare(0, 2, "//") == 0) {
                    hierarchy = AUTH;
                } else {
                    hierarchy = PATH;
                }
                this->URI_comp[SCHEME].first = true;
                this->URI_comp[SCHEME].second = token;
                break;
            case AUTH:
                tmp.erase(0, 2);
                hierarchy = tokenizeURI(tmp, token, "/?#", false);
                token.insert(0, "//");
                this->URI_comp[AUTH].first = true;
                this->URI_comp[AUTH].second = token;
                break;
            case PATH:
                hierarchy = tokenizeURI(tmp, token, "?#", false);
                this->URI_comp[PATH].first = true;
                this->URI_comp[PATH].second = token;
                break;
            case QUERY:
                hierarchy = tokenizeURI(tmp, token, "#", false);
                token.erase(0,1);
                this->URI_comp[QUERY].first = true;
                this->URI_comp[QUERY].second = token;
                break;
            case FRAG:
                this->URI_comp[FRAG].first = true;
                this->URI_comp[FRAG].second = tmp;
                hierarchy = END_URI;
                break;
        }
    }
    if (!this->URI_comp[SCHEME].first) {
        tmp = this->request_line[PROTOCOL];
        tokenizeURI(tmp, token, "/", true);
        this->URI_comp[SCHEME].first = true;
        this->URI_comp[SCHEME].second = string_to_lower(token);
    }

    if (!this->URI_comp[AUTH].first) {
        token = getHeaderValue("Host");
        if (token.size()) {
            token.insert(0, "//");
            this->URI_comp[AUTH].first = true;
            this->URI_comp[AUTH].second = token;
        }
    }
    return true;
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

bool    Request::validSCHEME( const std::string & token ) {
    size_t len = token.size();

    if (!isalpha(token[0]))
        return false;
    for (int i = 1; i < len; i++) {
        char c = token[i];
        if (!isalnum(c) && c != '+' && c != '-' && c != '.')
            return false;
    }
    return true;
}