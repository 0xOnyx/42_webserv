#ifndef GETNEXTLINE_HPP
# define GETNEXTLINE_HPP

# include "includes.h"
# include <string>
# include <iostream>
# include <fcntl.h>
# include <unistd.h>

# define GNL_BUFFER_SIZE 1024
# define NPOS std::string::npos

class GetNextLine {
	private:
		std::string	_data;
		std::string	_line;
		int			_fd;
		size_t		_pos;
		bool		_eof;

		GetNextLine( int fd );
		bool	extract( void );
	public:
		static GetNextLine*	init( int fd );
		~GetNextLine( void );

		std::string	getLine( void );
};

#endif