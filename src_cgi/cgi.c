#include <stdio.h>
#include <fcntl.h>

int	main(int argc, char **argv)
{
	struct s
	int	visits;
	int	fd;


	if ((fd = fd = open("number_of_visit.txt", O_RDWR)) < 0)
		return (1);
	if (fstat(fd, &stat) < 0)
	{
		close(fd);
		return (2);
	}
	if (lseek(fd, 0, SEEK_SET) != 0)
		return (3);
	return (0);
}