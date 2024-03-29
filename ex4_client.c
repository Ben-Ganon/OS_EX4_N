//Ben Ganon 318731007

#define _POSIX_SOURCE
#include <stdlib.h>
#include "stdio.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/signal.h>
#include "string.h"
#include <unistd.h>
#include <fcntl.h>
#include "errno.h"

void err_out();

void file_taken_hndlr(int x) {
    signal(SIGALRM, file_taken_hndlr);
}

void timeout_handler(int x) {
    signal(SIGALRM, timeout_handler);
    printf("Client closed because no response was received from the server for 30 seconds");
    err_out();
}

void ans_handler(int x) {
    signal(SIGUSR2, ans_handler);
    pid_t self = getpid();
    char pid_str[32] ={};
    char to_client[100] = {"to_client_"};
    sprintf(pid_str, "%d", (int)self);
    strcat(to_client, pid_str);
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
int get_rand(char rand_buff[7]){
    int x = 0;
    for (int i = 0; i < 7; ++i) {
        x += rand_buff[i] << i*8;
    }
    return abs(x) % 5 + 1;
}

int main(int argc, char* argv[]){
    char rand_buff[7] = {};
    pid_t self = getpid();
    char pid_buff[32]={};
    sprintf(pid_buff, "%d", self);

    if(argc < 5)
        err_out();

    pid_t serv_pid = atoi(argv[1]);
    if(serv_pid < 0)
        err_out();
    signal(SIGALRM, file_taken_hndlr);

    int i;
    int serv_fd;
    if(access("to_srv", F_OK) == 0){
        for (i = 0; i < 10; ++i) {
//            getrandom(rand_buff, 6,GRND_RANDOM);
            int x = rand()%5 + 1;
            alarm(x);
            pause();
            if(access("to_srv", F_OK) != 0)
                break;
        }
        if(i == 10)
            err_out();
        serv_fd = open("to_srv", O_CREAT | O_RDWR, 0666);
    } else {
        serv_fd = open("to_srv", O_CREAT | O_RDWR, 0666);
    }

    char * serv_req = strcat(pid_buff," ");
    serv_req = strcat(pid_buff,argv[2]);
    serv_req = strcat(pid_buff, " ");
    serv_req = strcat(pid_buff, argv[3]);
    serv_req = strcat(pid_buff, " ");
    serv_req = strcat(pid_buff, argv[4]);
    write(serv_fd, serv_req, strlen(serv_req));

    close(serv_fd);
    signal(SIGUSR2, ans_handler);
    signal(SIGALRM, timeout_handler);
    alarm(30);
    kill(serv_pid, SIGUSR1);
    pause();

    return 0;



}