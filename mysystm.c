#include "check.h"

int main(int argc, char* argv[]){
    //recombine cmd
    char cmd[1024]={0};
    for(int i=1;i<argc;i++){
        strcat(cmd,argv[i]);
        strcat(cmd," ");
    }
    //fork another process
    pid_t pid=fork();
    ERROR_CHECK(pid,-1,"fork");
    
    //launch bash and execute command
    if(pid==0){
        execlp("bash","bash","-c",cmd,NULL);
        _exit(127);
    } else { waitpid(pid,0,0); }

    return 0;
}
