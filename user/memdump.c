#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

void memdump(char *fmt, char *data){
  int p=0;
  unsigned char *udata=(unsigned char*)data;

  for(;*fmt;++fmt){
    switch (*fmt)
    {
    case 'i':{
    int n = 0;
    for(int i=0;i<4;++i){
      n|=((unsigned int)udata[p + i])<<i*8;
    }
    printf("%d\n",n);
    p+=4;
    break;
    }
    case 'p':{
    unsigned long long n = 0;
    for(int i=0;i<8;++i){
      n|=((unsigned long long)udata[p + i])<<i*8;
    }
    p+=8;
    printf("%llx\n",n);
      break;
    }
    case 'h':{
    short n=(udata[p+1]<<8)|udata[p];
    p+=2;
    printf("%d\n",n);
      break;
    }
    case 'c':{
    printf("%c\n", udata[p]);
    p+=1;
      break;
    }
    case 's':{
    unsigned long long n = 0;
    for(int i=0;i<8;++i){
      n|=((unsigned long long)udata[p + i])<<i*8;
    }
    p+=8;
    char* addr=(char*)n;
    printf("%s\n",addr);
      break;
    }
    case 'S':{
    printf("%s\n",&data[p]);
    p += strlen(&data[p]) + 1;
      break;
    }
    default:
      break;
    }
  }
}

int
main(int argc, char *argv[])
{
  if(argc == 1){
    printf("Example 1:\n");
    int a[2] = { 61810, 2025 };
    memdump("ii", (char*) a);
    
    printf("Example 2:\n");
    memdump("S", "a string");
    
    printf("Example 3:\n");
    char *s = "another";
    memdump("s", (char *) &s);

    struct sss {
      char *ptr;
      int num1;
      short num2;
      char byte;
      char bytes[8];
    } example;
    
    example.ptr = "hello";
    example.num1 = 1819438967;
    example.num2 = 100;
    example.byte = 'z';
    strcpy(example.bytes, "xyzzy");
    
    printf("Example 4:\n");
    memdump("pihcS", (char*) &example);
    
    printf("Example 5:\n");
    memdump("sccccc", (char*) &example);
  } else if(argc == 2){
    // format in argv[1], up to 512 bytes of data from standard input.
    char data[512];
    int n = 0;
    memset(data, '\0', sizeof(data));
    while(n < sizeof(data)){
      int nn = read(0, data + n, sizeof(data) - n);
      if(nn <= 0)
        break;
      n += nn;
    }
    memdump(argv[1], data);
  } else {
    printf("Usage: memdump [format]\n");
    exit(1);
  }
  exit(0);
}
