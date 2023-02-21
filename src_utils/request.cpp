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

Request::Request( const std::string & buffer )
{
	std::istringstream ss(buffer);
	std::string token;

	std::getline(ss, token);
	this->parseRequestLine(token);

	while(std::getline(ss, token)) {
		this->parseHeader(token);
	}
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

	if (!std::getline(ss, token, '/'))
		return false;
	if (token != "HTTP")
		return false;
	if (!std::getline(ss, token, '.'))
		return false;
	if (!is_a_number(token))
		return false;
	this->protocol[MAJOR] = std::atoi(token.data());
	if (!std::getline(ss, token, '.'))
		return false;
	if (!is_a_number(token))
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

void	Request::parseHeader( std::string header ) {
	std::istringstream ss(header);
	std::string key;
	std::string value;
	
	if (!header.compare(0, 1, " ") or !header.compare(0, 1, "	")) { 
		std::string	last_key = *this->_h_index.rbegin();
		std::getline(ss, value);
		value.erase(0, 1);
		this->_headers[last_key].append(value);
	} else {
		std::getline(ss, key, ':');
		this->_h_index.push_back(key);
		std::getline(ss, value);
		value.erase(0,1);
		this->_headers[key] = value;
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

std::map<std::string, std::string> &Request::getHeaders() {
	return this->_headers;
}