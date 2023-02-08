#if defined __linux__
# include "includes.h"

int poll_create(poll_t *poll)
{
	return ((poll->fd = epoll_create1(0)) == -1);
}

int poll_add(poll_t *poll, int fd, uint32_t events, void *data)
{
	struct	epoll_event event = {};

	event.events = events;
	event.data.ptr = data;
	return (epoll_ctl(poll->fd, EPOLL_CTL_ADD, fd, &event) == -1);
}

int poll_mod(poll_t *poll, int fd, uint32_t events, void *data)
{
	struct	epoll_event event = {};

	event.events = events;
	event.data.ptr = data;
	return ((epoll_ctl(poll->fd, EPOLL_CTL_MOD, fd, &event)) == -1);
}

int poll_del(poll_t *poll, int fd)
{
	return ((epoll_ctl(poll->fd, EPOLL_CTL_DEL, fd, NULL)) == -1);
}

int poll_wait(poll_t *poll, size_t max)
{
	struct epoll_event events[max];

	int count = epoll_wait(poll->fd, events, (int)max, -1);
	if (count == -1)
		return (1);
	for (int i = 0; i < count; i++)
		poll->cb(events[i].data.ptr, events[i].events, poll);
	return (0);
}

#endif
