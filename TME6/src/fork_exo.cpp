#include <iostream>
#include <unistd.h>

int main () {
	const int N = 3;
	int nbChildren = 0;
	std::cout << "main pid=" << getpid() << std::endl;
	int mainPid = getpid();
	for (int i=1, j=N; i<=N && j==N  ; i++ ) {
			if(fork()==0){
				nbChildren=0;
				
			}
			else{
				nbChildren++;
				break;
			}
			std::cout << " i:j " << i << ":" << j << std::endl;
			for (int k=1; k<=i && j==N ; k++) {
				if ( fork() == 0) {
					nbChildren=0;
					j=0;
					std::cout << " k:j " << k << ":" << j << std::endl;
				}
				else {
					nbChildren++;
				}
			}

		}
	
	for(int i=0; i<nbChildren; i++){
		wait(NULL);
	}
	if(getpid() == mainPid){
		std::cout<<"Fin du programme"<<std::endl;
	}
	return 0;
}
