#include <iostream>
#include <fstream>
#include <regex>
#include <chrono>

int countOccurences(std::string word, std::vector<std::pair<std::string,int>>const &p ){
	for(int i=0;i<p.size();++i){
		if(p[i].first==word){
			return p[i].second;
		}
	}
	return 0;


}

int main () {
	using namespace std;
	using namespace std::chrono;
	vector <string> words;
	ifstream input = ifstream("./WarAndPeace.txt");

	auto start = steady_clock::now();
	cout << "Parsing War and Peace" << endl;
	vector<pair<string,int>> pairs;
	size_t nombre_lu_differents = 0;
	// prochain mot lu
	string word;
	bool different = true;
	size_t nombre_lu = 0;
	// une regex qui reconnait les caractères anormaux (négation des lettres)
	regex re( R"([^a-zA-Z])");
	while (input >> word) {
		// élimine la ponctuation et les caractères spéciaux
		word = regex_replace ( word, re, "");
		// passe en lowercase
		transform(word.begin(),word.end(),word.begin(),::tolower);

		// word est maintenant "tout propre"
		
		if (nombre_lu % 100 == 0)
			// on affiche un mot "propre" sur 100
			cout << nombre_lu << ": "<< word << endl;
		for(int i=0;i<words.size();++i){
			if(words[i]==word){
				different=false;
				pairs[i].second++;
				break;
			}
		}
		if(different){
			words.push_back(word);
			pairs.push_back(make_pair(word,1));
			nombre_lu_differents++;
		}
		different=true;
		nombre_lu++;
	}
	input.close();

	cout << "Finished Parsing War and Peace" << endl;

	auto end = steady_clock::now();
    cout << "Parsing took "
              << duration_cast<milliseconds>(end - start).count()
              << "ms.\n";

    cout << "Found a total of " << nombre_lu << " words." << endl;
	cout << "Found a total of " << nombre_lu_differents << " different words." << endl;
	cout << "Found " << countOccurences("war",pairs) << " occurences of 'war'." << endl;
	cout << "Found " << countOccurences("peace",pairs) << " occurences of 'peace'." << endl;
	cout << "Found " << countOccurences("toto",pairs) << " occurences of 'toto'." << endl;
		
	


    return 0;
}


