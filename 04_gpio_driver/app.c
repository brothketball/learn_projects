#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int fd;
    char buf[64] = {0};
    fd = open("/dev/gpio_driver", O_RDWR);
    if (fd < 0) {
        perror("open error\r\n");
        return fd;
    }

    buf[0] = atoi(argv[1]);
    write(fd, buf, sizeof(buf));
    read(fd, buf, sizeof(buf));
    printf("buf is %d\r\n", buf[0]);

    close(fd);
    return 0;
}