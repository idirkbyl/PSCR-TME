#ifndef H_CHAT_COMMON
#define H_CHAT_COMMON

#define _XOPEN_SOURCE 700
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <ctype.h>

#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* Pour les constantes des modes */
#include <fcntl.h>           /* Pour les constantes O_* */
#include <semaphore.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <errno.h>

#include <string.h>

#define MAX_MESS 50
#define MAX_USERS 10
#define TAILLE_MESS 1024

struct message {
  long type;
  char content[TAILLE_MESS];
};

struct myshm {
  int read; /* nombre de messages retransmis par le serveur */
  int write; /* nombre de messages non encore retransmis par le serveur */
  int nb; /* nombre total de messages emis */
  sem_t sem;
  struct message messages[MAX_MESS];
};

char *getName(char *name);


void pushMessage(struct myshm *shm, long type, char *content){
  sem_wait(&shm->sem);
  shm->messages[shm->nb].type = type;
  strcpy(shm->messages[shm->write].content, content);
  shm->nb++;
  sem_post(&shm->sem);
}


#endif
