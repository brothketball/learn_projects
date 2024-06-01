#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int fd;
	char buf[32];
    fd = open(argv[1], O_RDWR);
    if (fd < 0) {
        perror("open error\r\n");
        return fd;
    }
    printf("open is ok\r\n");

	if (!strcmp(argv[2], "read")) {
		read(fd, buf, 32);
	} else if (!strcmp(argv[2], "write")) {
		write(fd, "hello\n", 6);
	}

    close(fd);
    return 0;
}
