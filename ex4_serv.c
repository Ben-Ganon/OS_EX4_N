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

void alrm_handler(int x){
    printf("The server was closed because no service request was received for the last 60 seconds\n");
    exit(-1);
}

void calc_req_handler(int x) {
    signal(SIGUSR1, calc_req_handler);
    char *args[] = {"ex4_serv", "CALC", NULL};
    pid_t pid = fork();
    if (pid == 0) {
        int retCode = execvp(args[0], args);
        if (retCode < 0) {
            perror("Error in: execvp");
            exit(-1);
        }
    } else {
        return;
    }
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

void operate(int op, int left, int right, int client_pid,int output_fd){
    if(op == 4 && right == 0)
        err_out();
    int result = 0;
    float res_div = 0;
    char result_str[CALC_SIZE] = {};
    if(op == 4){
        res_div = (float ) left / (float) right;
        sprintf(result_str, "%f", res_div);
        if( write(output_fd, result_str, strlen(result_str)) < 0)
            err_out();
        close(output_fd);
        kill(client_pid, SIGUSR2);
        return;
    }
    switch (op) {
        case 1:
            result = left + right;
        case 2:
            result = left - right;
        case 3:
            result = left * right;
    }

    sprintf(result_str, "%d", result);
    if( write(output_fd, result_str, strlen(result_str)) < 0)
        err_out();
    close(output_fd);
    kill(client_pid, SIGUSR2);
}

void calculate() {
    int client_fd = open("to_srv.txt", O_RDONLY);
    if(client_fd < 0)
        err_out();
    char calc_req[CALC_SIZE];
    read(client_fd,calc_req, CALC_SIZE);
    char* buff;
    buff = strtok(calc_req, " ");
    check_str(buff);
    char* client_pid_str = buff;
    client_pid_str = strcat("to_client_", client_pid_str);
    pid_t client_pid = atoi(buff);
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
    close(client_fd);
    if(remove("to_srv.txt") < 0)
       err_out();
    int output_fd = open(client_pid_str, O_RDWR | O_CREAT);
    if(output_fd < 0)
        err_out();
    if(operation < 1 || operation > 4)
            err_out();
    operate(operation, left, right, client_pid, output_fd);


    }


int main(int argc, char *args[]) {
    remove("to_srv.txt");
    if (argc > 1 && !strcmp(args[1], "CALC")) {
        calculate();
        return 0;
    }
    char cwd_buff[150];
    getcwd(cwd_buff, 150);
    char *tempPath = getenv("PATH");
    strcat(tempPath, ":");
    strcat(tempPath, cwd_buff);
    setenv("PATH", tempPath, 1);
    tempPath = getenv("PATH");
    printf("%s\n", tempPath);

    signal(SIGUSR1, calc_req_handler);
    signal(SIGALRM, alrm_handler);
    while(1) {
        alarm(60);
        pause();
    }
}
