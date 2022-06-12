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

void calculate() {
    int client_fd = open("to_serv.txt", O_RDONLY);
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
    char operation = *buff;
    buff = strtok(NULL, " ");
    check_str(buff);
    int right = atoi(buff);
    check_int(right);
    close(client_fd);
    if(remove("to_serv.txt") < 0){
        printf("ERROR_FROM_EX4\n");
        exit(-1);
    }
    open(client_pid_str, O_RDWR);
    switch (operation) {
        case '+':
            write()
        case '-':

        case '*':

        case '/':

        default:
            printf("ERROR_FROM_EX4\n");
            exit(1);
    }



    }

    int main(int argc, char *args[]) {
        if (remove("to_serv.txt") != 0){
            printf("ERROR_FROM_EX4\n");
            exit(-1);
        }

        if (!strcmp(args[1], "CALC")) {
            calculate();
            return 0;
        }
        char* cwd_buff;
        getcwd(cwd_buff, 150);
        char *tempPath = getenv("PATH");
        strcat(tempPath, ":");
        strcat(tempPath, cwd_buff);
        setenv("PATH", tempPath, 1);

        signal(SIGUSR1, calc_req_handler);
        signal(SIGALRM, alrm_handler);
        alarm(60);
        pause();
        pause();
        return 0;
    }
