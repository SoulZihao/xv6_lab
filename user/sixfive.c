#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int main(int argc, char *argv[])// 字符串数组
{
    if (argc < 2) {
        fprintf(2, "Usage: pause <ticks>\n");
        exit(1); 
    }
    for(int i=1;i<argc;++i){
        int fd = open(argv[i], O_RDONLY);
        if (fd < 0) {
            fprintf(2, "sixfive: cannot open %s\n", argv[1]);
            exit(1);
        }
        char c;
        char buf[64];       // 用于暂存数字字符串的缓冲区
        int len = 0;        // 当前缓冲区的长度
        int is_numeric = 1; // 标记当前这一段 token 是否是纯数字
        while(read(fd, &c, 1)==1){
            if (strchr(" -\r\t\n./,", c) != 0){ //分隔符
                if(len>0&&is_numeric==1){
                    buf[len] = '\0';
                    int num=atoi(buf);
                    if(num%5==0||num%6==0)printf("%d\n", num);
                }
                is_numeric=1;
                len=0;
            }else{// 数字或非法字符
                if(is_numeric==1&&c<='9'&&c>='0'){
                    buf[len]=c;
                    ++len;
                }else{
                    is_numeric=0;
                }
            }
        }
        if(len>0&&is_numeric==1){
            buf[len] = '\0';
            int num=atoi(buf);
            if(num%5==0||num%6==0)printf("%d\n", num);
        }
        close(fd);
    }
    exit(0);
}