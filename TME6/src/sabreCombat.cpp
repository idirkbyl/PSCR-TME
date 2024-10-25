#include <stdlib.h>
#include <iostream>
#include "rsleep.h"
#include <signal.h>
#include <unistd.h>

int vie=3;
void handler(int sig){
    vie--;
    std::cout << "le proceesus " << getpid() << " a perdu une vie " << std::endl;
    if(vie==0){
        std::cout << "le proceesus " << getpid() << "est mort " << std::endl;
        _exit(0);
    }
}

void coupPare(int sig){
    std::cout << "le proceesus " << getpid() << " a paré le coup " << std::endl;
}

void attaque(pid_t adversaire){
    struct sigaction attaque;
    memset(&attaque, 0, sizeof(attaque));
    attaque.__sigaction_u.__sa_handler = &handler;
    sigaction(SIGINT, &attaque, NULL);
    std::cout << "le processus " << getpid() << " attaque " << adversaire << std::endl;
    if(kill(adversaire, SIGINT) ==-1){
        std::cout << "le proceesus " << getpid() << " a tué son adversaire " << std::endl;
        exit(0);
    }
    randsleep();
}

void defense(){
    struct sigaction defense;
    memset(&defense, 0, sizeof(defense));
    defense.__sigaction_u.__sa_handler = &coupPare;
    sigaction(SIGINT, &defense, NULL);
    std::cout << "le processus " << getpid() << " se défend " << std::endl;
    randsleep();

}

void combat(pid_t adversaire){
    while(true){
        defense();
        attaque(adversaire);
        waitpid(adversaire, NULL, WNOHANG);
    }
}

int main(){
    pid_t adversaire = fork();
    if(adversaire==0){
        combat(getppid());
    }
    else{
        combat(adversaire);
    }
}
