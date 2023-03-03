#if defined __APPLE__
# include "includes.h"

int poll_create(poll_t *poll)
{
	return ((poll->fd = kqueue()) == -1);
}

int poll_add(poll_t *poll, int fd, uint32_t events, void *data)
{
	struct kevent change_event;
	EV_SET(&change_event, fd, events, EV_ADD, 0, 0, data);
	return (kevent(poll->fd, &change_event, 1, NULL, 0, NULL) == -1);
}

int poll_mod(poll_t *poll, int fd, uint32_t events, void *data)
{
	struct kevent change_event;
	EV_SET(&change_event, fd, events, EV_ADD, 0, 0, data);
	return (kevent(poll->fd, &change_event, 1, NULL, 0, NULL) == -1);
}

int poll_del(poll_t *poll, int fd)
{
	struct kevent change_event;
	EV_SET(&change_event, fd, EVFILT_WRITE | EVFILT_READ, EV_DELETE, 0, 0, NULL);
	return (kevent(poll->fd, &change_event, 1, NULL, 0, NULL) == -1);
}

int poll_wait(poll_t *poll, size_t max)
{
	struct kevent events[max];
	int count = kevent(poll->fd, NULL, 0, events, max, NULL);
	// Check if kevent returned prematurely.
	if (count == -1)
		return (1);
	try
	{
// Process the events.
		for (int i = 0; i < count; i++)
			poll->cb(events[i].udata,
					 events[i].flags & EV_ERROR ? events[i].flags : events[i].filter,
					 poll);
	}
	catch(std::exception const &e)
	{
		syslog(LOG_ERR, "failed to execute event");
	}
	return (0);
}

#endif