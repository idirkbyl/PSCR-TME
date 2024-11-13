#pragma once

#include <cstring> // size_t,memset
#include <semaphore.h> 
namespace pr {

#define STACKSIZE 100

template<typename T>
class Stack {
	T tab [STACKSIZE];
	size_t sz;
	sem_t semEmpty;
	sem_t semFull;
	sem_t mutex;
public :
	Stack () : sz(0) { 
		memset(tab,0,sizeof tab) ;
		sem_init(&semEmpty,1,0);
		sem_init(&semFull,1,STACKSIZE);
		sem_init(&mutex,1,1);
	}

	T pop () {
		// bloquer si vide
		sem_wait(&semEmpty);
		sem_wait(&mutex);
		T toret = tab[--sz];
		sem_post(&mutex);
		sem_post(&semFull);
		return toret;
	}

	void push(T elt) {
		//bloquer si plein
		sem_wait(&semFull);
		sem_wait(&mutex);
		tab[sz++] = elt;
		sem_post(&mutex);
		sem_post(&semEmpty);
	}
	~Stack(){
		sem_destroy(&semEmpty);
		sem_destroy(&semFull);
		sem_destroy(&mutex);
	}
};

}
