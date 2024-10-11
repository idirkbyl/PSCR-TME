#include "Banque.h"

#include <iostream>
#include <unistd.h>

using namespace std;

namespace pr {

void Banque::transfert(size_t deb, size_t cred, unsigned int val) {
	//unique_lock<mutex>l(m);
	Compte & debiteur = comptes[deb];
	Compte & crediteur = comptes[cred];
	//std::lock(debiteur.getMutex(), crediteur.getMutex());
	if(&debiteur.getMutex() < &crediteur.getMutex()){
		debiteur.getMutex().lock();
		crediteur.getMutex().lock();
	}else{
		crediteur.getMutex().lock();
		debiteur.getMutex().lock();
	}
	if (debiteur.debiter(val)) {
		crediteur.crediter(val);
	}
	//cout << "Transfert de " << val << "$ de " << cred << " vers " << deb << endl;
	debiteur.getMutex().unlock();
	crediteur.getMutex().unlock();
}
size_t Banque::size() const {
	return comptes.size();
}
bool Banque::comptabiliser (int attendu) const {
	//unique_lock<mutex>l(m);
	int bilan = 0;
	int id = 0;
	for (const auto & compte : comptes) {
		compte.getMutex().lock();
		if (compte.getSolde() < 0) {
			cout << "Compte " << id << " en négatif : " << compte.getSolde() << endl;
		}
		bilan += compte.getSolde();
		id++;
	}
	if (bilan != attendu) {
		cout << "Bilan comptable faux : attendu " << attendu << " obtenu : " << bilan << endl;
	}
	for(auto & compte : comptes){
		compte.getMutex().unlock();
	}
	return bilan == attendu;
}
}
