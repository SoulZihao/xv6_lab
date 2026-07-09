#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "kernel/riscv.h"

int is_alpha_numeric(char c) {
    return ((c >= 'a' && c <= 'z') || 
            (c >= 'A' && c <= 'Z') || 
            (c >= '0' && c <= '9'));
}

int
main(int argc, char *argv[])
{
  // Your code here.
  // 字符在 'a'~'z'、'A'~'Z' 或 '0'~'9' 之间
  char *start = sbrk(4096 * 20);
  if (start == (char*)-1) {
    exit(1); // 申请失败
  }
  char *end = start + (4096 * 20);
  char buf[256];
  int len=0;
  for(char*p=start;p<end;++p){
    if(is_alpha_numeric(*p)){
      buf[len++]=*p;
    }else{
      if (p < end && *p == '\0' && len >= 4) {
            // 抓到了！直接整行打印
            buf[len]='\0';
            printf("%s\n", buf);
            // exit(0); // 偷到秘密，功成身退
        }
        len=0;
    }
  }
  exit(1);
}
