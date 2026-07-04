#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])// 字符串数组
{
    if (argc < 2) {
        fprintf(2, "Usage: pause <ticks>\n");
        exit(1); 
    }
    int ticks = atoi(argv[1]);
    pause(ticks);
    exit(0);
}