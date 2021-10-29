#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int main()
{

    char *myfifo0 = "/tmp/myfifo0";
    mkfifo(myfifo0, 0666);
    int fd0 = open(myfifo0, O_WRONLY);

    char *myfifo1 = "/tmp/myfifo1";
    mkfifo(myfifo1, 0666);
    int fd1 = open(myfifo1, O_RDONLY);

    for (;;) {
        char s[20];
        fgets(s, 20, stdin);
        printf("Send '%s'\n", s);

        write(fd0, s, 20);

        unsigned int cnt_p;
        read(fd1, &cnt_p, 4);
        printf("Receive %u\n", cnt_p);
    }
}