#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

void find(char*path,char*name,int has_exec, char **exec_argv){
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;
    // memmove(buf,path,strlen(path));
    if((fd = open(path, O_RDONLY)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }
    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }
    if(st.type != T_DIR){
        fprintf(2, "find: %s is not a directory\n", path);
        close(fd);
        return;
    }
    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
        if(de.inum==0)continue;
        if(strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)continue;
        memmove(p,de.name,DIRSIZ);
        p[DIRSIZ]=0;
        if(stat(buf, &st) < 0){
            fprintf(2, "find: cannot stat %s\n", path);
            close(fd);
            continue;
        }
        switch(st.type){
        case T_DEVICE:
        case T_FILE:
            if(strcmp(name,de.name)==0){
                if(has_exec==0)printf("%s\n",buf);
                else{
                    int pid=fork();
                    if (pid < 0) {
                        fprintf(2, "fork failed\n");
                        exit(1);
                    }
                    if(pid==0){// child process
                        char *child_argv[MAXARG];
                        int len=0;
                        while(exec_argv[len] != 0 && len < MAXARG - 2){
                            child_argv[len] = exec_argv[len];
                            len++;
                        }
                        child_argv[len]=buf;
                        child_argv[len+1]=0;
                        exec(child_argv[0],child_argv);
                        fprintf(2, "exec %s failed\n", child_argv[0]);
                        exit(1);
                    }else{// main process
                        wait(0);
                    }
                }
            }
            break;
        case T_DIR:
            find(buf,name,has_exec,exec_argv);
            break;
        }
    }
}

int main(int argc, char *argv[]){// 注意第二个变量为字符指针数组
    if(argc<3){
        printf("Usage: %s path name [-exec exe param]\n",argv[0]);
        exit(1);
    }
    if(argc>3&&strcmp(argv[3],"-exec")!=0){
        printf("Usage: %s path name [-exec exe param]\n",argv[0]);
        exit(1);
    }
    int has_exec;
    char *exec_argv[MAXARG];
    has_exec=argc==3?0:1;
    for(int i=0;i<argc-4;++i){
        exec_argv[i]=argv[i+4];
    }
    exec_argv[argc-4]=0;
    find(argv[1],argv[2],has_exec,exec_argv);
    exit(0);
}