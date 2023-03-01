#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <alloca.h>
#include <sys/stat.h>
#include <string.h>

#define FILE "./cgi-bin/number_of_visit"
#define BUFFER_SIZE 4096

static void	html(int visits)
{
	char buff[BUFFER_SIZE];

	printf("Content-Type: text/html;\n\n");
	printf("<!DOCTYPE html>\n"
		   "<html>\n"
		   "<head>\n"
		   "    <title>Number of visits</title>\n"
		   "</head>\n"
		   "<body>\n"
		   "<h1>Number of visit %d</h1>\n"
		   "<p> REQUEST_URI => %s </p>"
		   "<p> PATH_INFO => %s </p>"
		   "<p> QUERY => %s </p>",
		   visits,
		   getenv("REQUEST_URI"),
		   getenv("PATH_INFO"),
		   getenv("QUERY_STRING"));

	printf("<p>content <br>");
	memset(buff, 0, BUFFER_SIZE);
	while (fgets((char *)buff, BUFFER_SIZE - 1, stdin) != NULL)
		printf("%s", buff);
	printf("</p>\n"
		   "</body>\n"
		   "</html>");
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
		if ((fd = open(FILE, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR)) < 0)
		{
			dprintf(2, "current user => %d\n ", getuid());
			perror("[ERROR]");
			return (1);
		}

	}
	else
	{
		if ((fd = open(FILE, O_RDWR)) < 0)
		{
			dprintf(2, "current user => %d\n ", getuid());
			perror("[ERROR]");
			return (2);
		}
		if (fstat(fd, &stat_file) < 0)
		{
			close(fd);
			perror("[ERROR]");
			return (3);
		}
		res = (char *) alloca((size_t) stat_file.st_size);
		if (read(fd, res, stat_file.st_size) < 0)
		{
			close(fd);
			perror("[ERROR]");
			return (4);
		}
		sscanf(res, "%d", &visits);
	}
	if (lseek(fd, 0, SEEK_SET) != 0)
	{
		close(fd);
		perror("[ERROR]");
		return (5);
	}
	html(visits);
	visits++;
	dprintf(fd, "%d", visits);
	close(fd);
	return (0);
}