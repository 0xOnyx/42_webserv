#include "includes.h"

void	set_nonblocking(int sockfd)
{
	int	current_flag;

	if ((current_flag = fcntl(sockfd, F_GETFD, 0)) == -1
		|| fcntl(sockfd, F_SETFD, current_flag | O_NONBLOCK) == -1)
		throw std::runtime_error(strerror(errno));
}
