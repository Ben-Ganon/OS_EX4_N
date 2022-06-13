//Ben Ganon 318731007
//
#include <stdlib.h>
#include "stdio.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/signal.h>
#include "string.h"
#include <unistd.h>
#include <fcntl.h>

#define CALC_SIZE 200

int main(int argc, char *args[]);
void calculate();

void alrm_handler(int x){
    printf("The server was closed because no service request was received for the last 60 seconds\n");
    exit(-1);
}

void calc_req_handler(int x) {
    signal(SIGUSR1, calc_req_handler);
    pid_t pid = fork();
    if (pid == 0) {
      calculate();
    } else {
        return;
    }
}

void check_str(char* buff){
    if(buff == NULL){
        printf("ERROR_FROM_EX4\n");
        exit(-1);
    }
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

void operate(int op, int left, int right, int client_pid,int output_fd){
    if(op == 4 && right == 0){
        close(output_fd);
        err_out();
    }
    int result = 0;
    float res_div = 0;
    char result_str[CALC_SIZE] = {};
    if(op == 4) {
        res_div = (float) left / (float) right;
        sprintf(result_str, "%f", res_div);
        if (write(output_fd, result_str, strlen(result_str)) < 0)
            err_out();
        close(output_fd);
        kill(client_pid, SIGUSR2);
        return;
    } else if(op == 1)
        result = left + right;
    else if(op ==2)
        result = left - right;
    else if(op ==3)
        result = left * right;

    sprintf(result_str, "%d", result);
    if( write(output_fd, result_str, strlen(result_str)) < 0)
        err_out();
}

void calculate() {
    int client_fd = open("to_srv.txt", O_RDONLY);
    if(client_fd < 0)
        err_out();
    char calc_req[CALC_SIZE]= {};
    if(read(client_fd,calc_req, CALC_SIZE) <0)
        err_out();
    char* buff;
    buff = strtok(calc_req, " ");
    check_str(buff);
    int client_pid = atoi(buff);

    int buff_len = strlen(buff);
    char client_pid_str[buff_len];
    strcpy(client_pid_str, buff);
    char to[] = "to_client_";
    strcat(to, client_pid_str);
    check_int(client_pid);


    buff = strtok(NULL, " ");
    check_str(buff);
    int left = atoi(buff);
    check_int(left);
    buff = strtok(NULL, " ");
    check_str(buff);
    int operation = atoi(buff);
    check_int(operation);
    buff = strtok(NULL, " ");
    check_str(buff);

    int right = atoi(buff);
    check_int(right);
    if(close(client_fd) <0)
        err_out();
    if(remove("to_srv.txt") < 0)
       err_out();
    int output_fd = open(to, O_RDWR | O_CREAT, 0666);
    if(output_fd < 0)
        err_out();
    if(operation < 1 || operation > 4)
            err_out();
    operate(operation, left, right, client_pid, output_fd);
    close(output_fd);
    kill(client_pid, SIGUSR2);
    exit(0);
}


int main(int argc, char *args[]) {
    if(access("to_srv.txt", F_OK) == 0){
        remove("to_srv.txt");
    }

    printf("server pid: %d\n", (int)getpid());
//    char cwd_buff[150];
//    getcwd(cwd_buff, 150);
//    char *tempPath = getenv("PATH");
//    strcat(tempPath, ":");
//    strcat(tempPath, cwd_buff);
//    setenv("PATH", tempPath, 1);

    signal(SIGUSR1, calc_req_handler);
    signal(SIGALRM, alrm_handler);
    while(1) {
        alarm(120);
        pause();
    }
}
