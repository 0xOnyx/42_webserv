#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <alloca.h>
#include <sys/stat.h>

#define FILE "./src_cgi/number_of_visit"

static void	html(int visits)
{
	printf("Content-Type: text/html;\n\n");
	printf("<!DOCTYPE html>\n"
		   "<html>\n"
		   "<head>\n"
		   "    <title>Number of visits</title>\n"
		   "</head>\n"
		   "<body>\n"
		   "<h1>Number of visit %d</h1>\n"
		   "</body>\n"
		   "</html>", visits);
}

int	main(void)
{
	char *res;
	struct stat stat_file;
	int visits;
	int fd;

	visits = 0;
	if (access(FILE, F_OK) == -1)
	{
		if ((fd = open(FILE, O_RDWR | O_CREAT)) < 0)
			return (1);
	}
	else
	{
		if ((fd = open(FILE, O_RDWR)) < 0)
			return (2);
		if (fstat(fd, &stat_file) < 0)
		{
			close(fd);
			return (3);
		}
		res = (char *) alloca((size_t) stat_file.st_size);
		if (read(fd, res, stat_file.st_size) < 0)
		{
			close(fd);
			return (4);
		}
		sscanf(res, "%d", &visits);
	}
	if (lseek(fd, 0, SEEK_SET) != 0)
	{
		close(fd);
		return (4);
	}
	html(visits);
	visits++;
	dprintf(fd, "%d", visits);
	return (0);
}