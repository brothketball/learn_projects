#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int fd;
    char buf[64] = "abaabaaba   ";
    fd = open("/dev/file_operation", O_RDWR);
    if (fd < 0) {
        perror("open error\r\n");
        return fd;
    }
    write(fd, buf, sizeof(buf));
    read(fd, buf, sizeof(buf));
    printf("buf is %s\r\n", buf);

    close(fd);
    return 0;
}