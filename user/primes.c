#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int leftToCurrent[2];
    int currentToRight[2];
    pipe(leftToCurrent);

    int nextNumber = 0;
    int nowNumber = 0;
    int toRight = 0;

    int pid = fork();
    if (pid != 0) {
        for (int i = 2; i <= 35; i++) {
            write(leftToCurrent[1], &i, sizeof(int));
        }
    } else {
        close(leftToCurrent[1]); 
        while(read(leftToCurrent[0], &nextNumber, sizeof(int))) {
            if (nowNumber == 0) {
                nowNumber = nextNumber;
                fprintf(2, "prime %d\n", nowNumber);
            } else if (nextNumber % nowNumber != 0) {
                if (toRight == 0) {
                    pipe(currentToRight);
                    pid = fork();
                    if (pid != 0) {
                        toRight = 1;
                    } else {
                        nowNumber = 0;
                        close(currentToRight[1]);
                        leftToCurrent[0] = currentToRight[0];
                    }
                } 
                write(currentToRight[1], &nextNumber, sizeof(int));
            }
        }
    }
    close(leftToCurrent[0]);
    close(leftToCurrent[1]);
    close(currentToRight[0]);
    close(currentToRight[1]);
    wait(0);
    exit(0);
}
