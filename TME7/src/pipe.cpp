#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_ARGS 100

int main(int argc, char *argv[]) {
    int pipe_index = -1;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "|") == 0) {
            pipe_index = i;
            break;
        }
    }

    if (pipe_index == -1) {
        fprintf(stderr, "Erreur : pas de pipe trouvé\n");
        return 1;
    }

    // Séparer les commandes avant et après le pipe
    char *cmd1[MAX_ARGS], *cmd2[MAX_ARGS];
    int cmd1_len = 0, cmd2_len = 0;

    for (int i = 1; i < pipe_index; i++) {
        cmd1[cmd1_len++] = argv[i];
    }
    cmd1[cmd1_len] = nullptr;

    for (int i = pipe_index + 1; i < argc; i++) {
        cmd2[cmd2_len++] = argv[i];
    }
    cmd2[cmd2_len] = nullptr;

    // Créer un pipe
    int pipe_fd[2];
    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        return 1;
    }

    // Créer le producteur
    pid_t pid1 = fork();
    if (pid1 == -1) {
        perror("fork");
        return 1;
    }

    if (pid1 == 0) {  // fils (1er commande)
        // Rediriger la sortie standard vers le pipe
        if (dup2(pipe_fd[1], STDOUT_FILENO) == -1) {
            perror("dup2");
            return 1;
        }
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        // Exécuter la première commande
        if (execv(cmd1[0], cmd1) == -1) {
            perror("execv");
            return 1;
        }
    }

    // Créer le consommateur
    pid_t pid2 = fork();
    if (pid2 == -1) {
        perror("fork");
        return 1;
    }
    if (pid2 == 0) {  //  fils (2ème commande)
        // Rediriger l'entrée standard depuis le pipe
        if (dup2(pipe_fd[0], STDIN_FILENO) == -1) {
            perror("dup2");
            return 1;
        }
        close(pipe_fd[1]);
        close(pipe_fd[0]);
        // Exécuter la deuxième commande
        if (execv(cmd2[0], cmd2) == -1) {
            perror("execv");
            return 1;
        }
    }
    // père
    close(pipe_fd[0]);
    close(pipe_fd[1]);

    // Attendre fils 
    wait(NULL);
    wait(NULL);

    return 0;
}
