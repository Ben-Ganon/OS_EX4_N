//Ben Ganon 318731007
#include <stdlib.h>
#include "stdio.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/signal.h>
#include "string.h"
#include <unistd.h>
#include <fcntl.h>
#include "errno.h"
#include <sys/random.h>

void alrm_handlr(int x) {
    signal(SIGALRM, alrm_handlr);
}

void check_str(char* buff){
    if(buff == NULL){
        printf("ERROR_FROM_EX4\n");
        exit(-1);
    }

    return;
}
void check_int(int num){
    if(num <= 0){
        printf("ERROR_FROM_EX4\n");
        exit(-1);
    }

    return;
}
void err_out(){
    printf("ERROR_FROM_EX4\n");
    exit(-1);
}

void get_args(char* argv[], int* pid, ){

}

int main(int argc, char* argv[]){
    char rand_buff[6];
    if(argc < 5)
        err_out();
    char* buff;
    buff = argv[1];
    pid_t serv_pid = atoi(buff);
    check_int(serv_pid);
    buff = argv[2];
    int left = atoi(buff);
    if(left < 0 && !strcmp(buff, "0"))
        err_out();
    buff = argv[3];
    int operator = atoi(buff);
    if(operator < 1 || operator > 4)
        err_out();
    int right = atoi(argv[4]);
    if(right < 0 && !strcmp(buff, "0"))
        err_out();
    signal(SIGALRM, alrm_handlr);
    int serv_fd = open("to_srv.txt", O_CREAT | O_RDWR);
    int i;
    if(errno == EACCES){
        for (i = 0; i < 10; ++i) {
            alarm((getrandom(rand_buff, 6,GRND_RANDOM))%6);
            pause();
            serv_fd = open("to_srv.txt", O_CREAT | O_RDWR);
            if(errno != EACCES && serv_fd > 0)
                break;
        }
        if(i == 9 && serv_fd < 0)
            err_out();
    }



}