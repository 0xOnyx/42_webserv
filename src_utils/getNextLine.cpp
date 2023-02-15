#include "includes.h"

GetNextLine*	GetNextLine::init( int fd ) {
	if (fd < 0)
		return nullptr;
	return new GetNextLine(fd);
}

GetNextLine::GetNextLine( int fd ) : _fd(fd), _pos(NPOS) {
	this->_data.clear();
}

GetNextLine::~GetNextLine( void ) {}

bool GetNextLine::extract( void ) {
	char	*buffer = new char[GNL_BUFFER_SIZE];

	if (buffer == nullptr)
		return nullptr;
	int len = 1;
	while ( len > 0 and this->_pos == NPOS) {
		len = read(this->_fd, buffer, GNL_BUFFER_SIZE);
		if (len <= 0) {
			delete [] buffer;
			return false;
		}
		this->_data.append(buffer, len);
		this->_pos = this->_data.find('\n');
	}
	delete [] buffer;
	return true;
}

std::string GetNextLine::getLine( void ) {
	this->_line.clear();
	this->_pos = this->_data.find('\n');
	if (this->_pos != NPOS) {
		this->_line = this->_data.substr(0, this->_pos + 1);
		this->_data.erase(0, this->_pos + 1);
	}
	else if (this->extract() == true) {
		this->_line = this->_data.substr(0, this->_pos + 1);
		this->_data.erase(0, this->_pos + 1);
	}
	else if (!(this->_data.empty())) {
		this->_line = this->_data;
		this->_data.clear();
	}
	return this->_line;
}