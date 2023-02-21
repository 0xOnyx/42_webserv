#ifndef UTILS_H
# define UTILS_H
# include "includes.h"

struct s_event
{
	int 			fd;
	class	Socket	*socket;
};

# if defined __linux__
	#include <sys/epoll.h>
	#include <sys/signalfd.h>
	#include <stddef.h>

	typedef struct poll_t poll_t;
	typedef void (*poll_cb_t)(void *data, int events, poll_t *poll);

	struct poll_t
	{
		poll_cb_t   cb;
		void        *data;
		int         fd;
	};

	int poll_create(poll_t *poll);
	int poll_add(poll_t *poll, int fd, uint32_t events, void *data);
	int poll_mod(poll_t *poll, int fd, uint32_t events, void *data);
	int poll_del(poll_t *poll, int fd);
	int poll_wait(poll_t *poll, size_t max);
# endif


# if defined __APPLE__
	#include <sys/event.h>
	#include <sys/signal.h>

	typedef struct poll_t poll_t;
	typedef void (*poll_cb_t)(void *data, int events, poll_t *poll);

	struct poll_t
	{
		poll_cb_t cb;
		void *data;
		int fd;
	};

	int poll_create(poll_t *poll);
	int poll_add(poll_t *poll, int fd, uint32_t events, void *data);
	int poll_mod(poll_t *poll, int fd, uint32_t events, void *data);
	int poll_del(poll_t *poll, int fd);
	int poll_wait(poll_t *poll, size_t max);
# endif

void	handler_poll(void *data, int event, poll_t *poll);
typedef std::pair<std::string, std::string> pair_str;
void	set_nonblocking(int sockfd);
size_t	get_size_file(int fd);

#endif
