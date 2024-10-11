#include "Banque.h"
#include <iostream>
#include <random>

using namespace std;

int ThreadSpleRand(int min, int max) {
	static thread_local std::mt19937 generator;
	std::uniform_int_distribution<int> distribution(min, max);
	return distribution(generator);
}

void threadFunction(pr::Banque & banque) {
	for (int k = 0; k < 1000; k++) {
		int i = ThreadSpleRand(0, banque.size()-1);
		int j = ThreadSpleRand(0, banque.size()-1);
		int m = ThreadSpleRand(0, 100);
		banque.transfert(i, j, m);
		//std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 20));
	}
}

const int NB_THREAD = 10;
int main () {
	vector<thread> threads;
	pr::Banque banque(10, 100);
	// TODO : creer des threads qui font ce qui est demandé
	for(int i=0;i<NB_THREAD;i++){
		threads.push_back(thread(threadFunction, ref(banque)));
	}

	thread t([&banque]() { 
		if(banque.comptabiliser(1000)){
			cout << "Bilan comptable correct" << endl;
		};
		});
	
	t.join();
	for (auto & t : threads) {
		t.join();
	}

	
	return 0;
}
