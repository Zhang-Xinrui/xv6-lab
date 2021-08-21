#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    char buf[512];
    int bufAddr = 0;
    int pid;
    while (read(0, &buf[bufAddr], 1)) { 
        if (buf[bufAddr] == '\n') {
            pid = fork();
            bufAddr = 0;
            if (pid == 0) {
                char *arg[MAXARG];
                int i;
                for (i = 1; i < argc; i++) {
                    arg[i - 1] = argv[i];
                }
                int lastAddr = bufAddr;
                while (buf[bufAddr] != '\n') {
                    if ((buf[bufAddr] == '\0') || (buf[bufAddr] == ' ')) {
                        arg[i - 1] = &buf[lastAddr]; i++;
                        lastAddr = bufAddr + 1;
                    }
                    bufAddr++;
                }
                buf[bufAddr] = '\0';
                arg[i - 1] = &buf[lastAddr];
                exec(argv[1], arg);
            } else {
                wait(0);
            }
        } else {
            bufAddr++;
        }
    }
    exit(0);
}
