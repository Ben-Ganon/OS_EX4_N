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

void err_out();

void alrm_handlr(int x) {
    signal(SIGALRM, alrm_handlr);
}

void ans_handlr(int x) {
    signal(SIGUSR2, ans_handlr);

    pid_t self = getpid();
    printf("this pid is: %d\n", self);
    char pid_str[32] ={};
    char to_client[100] = {"to_client_"};
    sprintf(pid_str, "%d", (int)self);
    strcat(to_client, pid_str);
    printf("opening: %s\n", to_client);
    int calc_fd = open(to_client, O_RDONLY);
    if(calc_fd < 0)
        err_out();
    char buff[100] ={};
    read(calc_fd, buff, 100);
    close(calc_fd);
    remove(to_client);
    printf("%s\n", buff);

    exit(-1);
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


int main(int argc, char* argv[]){
    char rand_buff[6];
    pid_t self = getpid();
    char pid_buff[32]={};
    sprintf(pid_buff, "%d", self);

    if(argc < 5)
        err_out();

    pid_t serv_pid = atoi(argv[1]);
    if(serv_pid < 0)
        err_out();
    signal(SIGALRM, alrm_handlr);
    int serv_fd = open("to_srv.txt", O_CREAT | O_RDWR, 0666);
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

    char * serv_req = strcat(pid_buff," ");
    serv_req = strcat(pid_buff,argv[2]);
    serv_req = strcat(pid_buff, " ");
    serv_req = strcat(pid_buff, argv[3]);
    serv_req = strcat(pid_buff, " ");
    serv_req = strcat(pid_buff, argv[4]);
    printf("writing: %s\n", serv_req);
    write(serv_fd, serv_req, strlen(serv_req));

    close(serv_fd);
    signal(SIGUSR2, ans_handlr);
    alarm(30);
    kill(serv_pid, SIGUSR1);
    pause();

    return 0;



}