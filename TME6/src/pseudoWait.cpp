#include <stdlib.h>
#include <iostream>
#include "rsleep.h"
#include <signal.h>
#include <unistd.h>
int wait_till_pid(pid_t pid){
    int status;
    while(true){
        pid_t w = wait(&status);
        if(w == pid){
            return w;
        }
        if(w == -1){
            if(errno == ECHILD){
                return -1;
            }
        }
    }
}

int childFinished = 0;
void alarmHandler(int sig){
    std::cout << "j'en ai assez d'attendre" << std::endl;
    _exit(0);
}

void childHandler(int sig){
    
}

int wait_till_pid(pid_t pid, int sec){


    struct sigaction actAlarm;
    memset(&actAlarm, 0, sizeof(actAlarm));
    actAlarm.__sigaction_u.__sa_handler = &alarmHandler;
    sigaction(SIGALRM, &actAlarm, NULL);
    
    struct sigaction actChild;
    memset(&actChild, 0, sizeof(actChild));
    actChild.__sigaction_u.__sa_handler = &childHandler;
    sigaction(SIGCHLD, &actChild, NULL);

    alarm(sec);

    sigset_t mask;
    sigfillset(&mask);
    sigdelset(&mask, SIGCHLD);
    sigdelset(&mask, SIGALRM);
    while(true){
        sigsuspend(&mask);
        pid_t p=wait(nullptr);
        if(p == pid){
            std::cout << "mon fils est enfin mort" << std::endl;
            return p;
        }
    }

}


int main(){
    pid_t pid = fork();
    if(pid == 0){
        sleep(10);
        exit(0);
    }
    else{
        wait_till_pid(pid, 5);
    }
    return 0;
}