#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int main()
{

    char *myfifo0 = "/tmp/myfifo0";
    mknod(myfifo0, S_IFIFO|0666, 0);
    int fd0 = open(myfifo0, O_WRONLY|O_CREAT);

    char *myfifo1 = "/tmp/myfifo1";
    mknod(myfifo1, S_IFIFO|0666, 0);
    int fd1 = open(myfifo1, O_RDONLY|O_CREAT);

    printf("!\n");

    for (;;) {
        char s[20];
        fgets(s, 20, stdin);
        printf("Send '%s'\n", s);

        write(fd0, s, 20);

        unsigned int cnt_p = 10;
        int c = read(fd1, &cnt_p, 4);
        printf("Receive %u %d\n", cnt_p, c);
    }
}