#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int fd;
    fd = open("/dev/device_test", O_RDWR);
    if (fd < 0) {
        perror("open error\r\n");
        return fd;
    }
    // printf("buf is %s\r\n", buf);

    close(fd);
    return 0;
}
