#include "Stack.h"
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <sys/mman.h>
#include <fcntl.h>

using namespace std;
using namespace pr;


bool isFinished = false;
std::vector<pid_t> vec(1000);
void handler(int sig){
	isFinished = true;
}

void handler2(int sig){
	for(pid_t pid : vec){
		kill(pid,SIGUSR1);
	}
}
void producteur (Stack<char> * stack) {
	char c ;
	while (cin.get(c) && (! isFinished)) {
		stack->push(c);
	}
}

void consomateur (Stack<char> * stack) {
	while (!isFinished) {
		char c = stack->pop();
		if(!c) return;
		cout << c << endl ;
	}
}


//Stack<char> * s
int main (int argc, char** argv) {
	// int fd= shm_open("/stack",O_RDWR|O_CREAT,0600);
	// if(fd==-1){
	// 	perror("shm_open");
	// 	exit(1);
	// }
	// if(ftruncate(fd,sizeof(Stack<char>))==-1){
	// 	perror("ftruncate");
	// 	exit(1);
	// }
	// if(s=(Stack<char> *) mmap(NULL, sizeof(Stack<char>), PROT_READ |PROT_WRITE, MAP_SHARED, fd, 0)== MAP_FAILED) {
	// 		perror("mmap");
	// 		exit(1);
	// }
	if(argc < 3 ){
		cout << "Usage ./prod_cons <nbProd> <nbCons>" << endl;
		return 0; 
	}

	int nbProd = atoi(argv[1]);
	int nbCons = atoi(argv[2]);


	//Interrompre les consommateurs lors du ctrl-c : 
	struct sigaction sig;
	memset(&sig, 0, sizeof(sig));
	sig.sa_handler= &handler;
	sigaction(SIGUSR1,&sig,NULL);

	struct sigaction sig2;
	memset(&sig2, 0, sizeof(sig2));
	sig2.sa_handler= &handler2;
	sigaction(SIGINT,&sig2,NULL);

	

	void *ptr = mmap(0,sizeof(Stack<char>),PROT_READ|PROT_WRITE,MAP_ANONYMOUS|MAP_SHARED,-1,0);
	Stack<char> * s = new (ptr)Stack<char>();

	for(int i=0;i<nbProd;i++){
		pid_t pp = fork();
		if (pp==0) {
			producteur(s);
			exit(0);
		}
	}
	for(int i=0;i<nbCons;i++){
		pid_t pc = fork();
		if (pc==0) {
			consomateur(s);
			exit(0);
		}
	}

	for(int i=0;i<(nbCons+nbProd);i++){
		wait(0);
	}
	s->~Stack();
	munmap(ptr,sizeof(Stack<char>));
	return 0;
}

