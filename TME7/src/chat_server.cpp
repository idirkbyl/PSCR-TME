#include "chat_common.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <algorithm>
struct myshm * myShm;
char servername[50];
    void handler(int sig){
        sem_destroy(&myShm->sem);
        sem_unlink(servername);
        _exit(0);
    }


    int main(int argc, char **argv){
        signal(SIGINT, handler);
        //verification du bon nombre d'arguments
        if(argc != 2){
            std::cout << "Usage: " << argv[0] << " <name>" << std::endl;
            return 1;
        }
        
        strcpy(servername, "/");
        strcat(servername, argv[1]);

      

        //Allocation du segment de memoire partag serveur
        int fd= shm_open(servername, O_CREAT | O_EXCL | O_RDWR, 0600);
        if(fd == -1){
            perror("shm_open");
            return 1;
        }
        ftruncate(fd,sizeof(struct myshm ));

        myShm = (myshm *) mmap(NULL,sizeof(struct myshm),
                                    PROT_READ|PROT_WRITE,
                                    MAP_SHARED,fd,0);

        sem_init(&(myShm->sem),1,1);

        close(fd);
        printf("Le serveur à alloué son segment de memoire partagé\n");
        typedef std::pair<char *, struct myshm*> ppair;
        std::vector<ppair> vecShm; 

        while(true){
            sem_wait(&(myShm->sem));
            int nbMess = myShm->nb;
            for(int i=0;i<nbMess;++i){
                
                struct message m = myShm->messages[i];
                long type = m.type;
                switch(type){
                    case 1: //connexion
                    {
                        
                        std::cout << m.content<< " has joined the chat" << std::endl;
                        char buff[256];
                        strcpy(buff,m.content);
                        strcpy(buff+strlen(m.content)," has joined the chat");
                        int fd= shm_open(m.content,O_RDWR,0600);
                        if(fd == -1){
                            perror("shm_open");
                            return 1;
                        }
                        ftruncate(fd,sizeof(struct myshm ));
                        struct myshm * myShared= (struct myshm *) mmap(0,sizeof(struct myshm),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
                        close(fd);
                        if(myShared == MAP_FAILED){
                            perror("mmap");
                            return 1;
                        }

                        vecShm.push_back(std::make_pair(m.content,myShared));
                        for(auto &s : vecShm){
                            pushMessage(s.second,m.type,buff);
                        }
                        break;


                    }
                    case 2: // diffusion message
                    {
                        for(auto &s : vecShm){
                            pushMessage(s.second,m.type,m.content);
                        }

                        break;


                    }
                    case 3: //deconnection
                    {
                        //TODO : vec.erase(std::find(vec.begin(), vec.end(), m.content
                        for(int i=0;i<vecShm.size();++i){
                            if(strcmp(vecShm[i].first, m.content)==0){
                                vecShm.erase(vecShm.begin() + i);
                                break;
                            }
                        }
                        std::cout << m.content << " has left the chat" << std::endl;
                        char buff[256];
                        strcpy(buff,m.content);
                        strcpy(buff+strlen(m.content),"has left the chat");
                        for(auto &s : vecShm){
                            pushMessage(s.second,m.type,m.content);
                        }
                        break;
                    default:
                        std::cout << "Unknown message type" << std::endl;
                        break;
                    }
                }

            }
            myShm->nb = 0;
            sem_post(&(myShm->sem));
        }
        return 0;
}