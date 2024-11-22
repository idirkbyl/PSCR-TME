#include "chat_common.h"
#include <iostream>
#include <thread>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

struct myshm *shm_client;
struct myshm *shm_server;
char name[50];

void handle_sigint(int sig) {
    std::cout << "Caught signal " << sig << ", exiting..." << std::endl;
    char disconnect_message[TAILLE_MESS];
    snprintf(disconnect_message, TAILLE_MESS, "/%s", name);
    pushMessage(shm_server, 3, disconnect_message);
    printf("%p\n",shm_client);
    printf("%p\n",shm_server);
    munmap(shm_client, sizeof(struct myshm));
    munmap(shm_server, sizeof(struct myshm));



    _exit(0);
}

void read_from_server() {
    while (true) {
        sem_wait(&(shm_client->sem));
        for (int i = 0; i < shm_client->nb; ++i) {
            std::cout << "Message from server: " << shm_client->messages[i].content << std::endl;
        }
        shm_client->nb = 0;
        sem_post(&(shm_client->sem));
    }
}

void write_to_server() {
    std::cout << "Starting write_to_server thread..." << std::endl;
    char buffer[TAILLE_MESS];
    while (true) {
        std::cin.getline(buffer, TAILLE_MESS);
        std::cout << "Sending message to server: " << buffer << std::endl;
        pushMessage(shm_server, 2, buffer);
    }
}



int main(int argc, char **argv) {
    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " <client_shm_name> <server_shm_name>" << std::endl;
        return 1;
    }

    
    strncpy(name,argv[1],strlen(argv[1]));
    std::cout << "Setting up signal handler..." << std::endl;
    signal(SIGINT, handle_sigint);

    std::cout << "Opening client shared memory..." << std::endl;
    int fd_client = shm_open(argv[1], O_RDWR | O_CREAT | O_EXCL, 0600);
    if (fd_client == -1) {
        perror("shm_open client");
        return 1;
    }
    ftruncate(fd_client, sizeof(struct myshm));
    
    shm_client = (struct myshm *)mmap(NULL, sizeof(struct myshm), PROT_READ | PROT_WRITE, MAP_SHARED, fd_client, 0);
    sem_init(&shm_client->sem,1,1);
    close(fd_client);

    std::cout << "Opening server shared memory..." << std::endl;
    int fd_server = shm_open(argv[2], O_RDWR, 0600);
    if (fd_server == -1) {
        perror("shm_open server");
        return 1;
    }
    ftruncate(fd_server, sizeof(struct myshm));
    
    shm_server = (struct myshm *)mmap(NULL, sizeof(struct myshm), PROT_READ | PROT_WRITE, MAP_SHARED, fd_server, 0);
    
    //connection message to server
    std::cout << "Sending connection message to server..." << std::endl;
    char connect_message[TAILLE_MESS];
    snprintf(connect_message, TAILLE_MESS, "/%s", argv[1]);
    pushMessage(shm_server, 1, connect_message);
    
    close(fd_server);

    std::cout << "Starting reader and writer threads..." << std::endl;
    std::thread reader(read_from_server);
    std::thread writer(write_to_server);

    reader.join();
    writer.join();

    return 0;
}