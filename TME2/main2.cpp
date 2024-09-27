#include "hashmap.hpp"
#include <iostream>
#include <fstream>
#include <regex>
#include <chrono>
#include <forward_list>

int main () {
	using namespace std;
	using namespace std::chrono;
	HashMap<string,int> map(1000);
	ifstream input = ifstream("./WarAndPeace.txt");
	auto start = steady_clock::now();
	std::cout << "Parsing War and Peace" << endl;
	// prochain mot lu
	string word;
	size_t nombre_lu = 0;

	// une regex qui reconnait les caractères anormaux (négation des lettres)
	regex re( R"([^a-zA-Z])");
	while (input >> word) {
		// élimine la ponctuation et les caractères spéciaux
		word = regex_replace ( word, re, "");

		// passe en lowercase

		transform(word.begin(),word.end(),word.begin(),::tolower);

		// word est maintenant "tout propre"


        map.insert(word, 1);
        
		if (nombre_lu % 100 == 0)
			// on affiche un mot "propre" sur 100
			std::cout << nombre_lu << ": "<< word << endl;

		nombre_lu++;
	}
	input.close();

    vector<pair<string,int>> pairs;
    for(auto &bucket : map.getBuckets()){
        for(auto &entry : bucket){
            pairs.push_back(make_pair(entry.key, entry.value));
        }
    }
    std::sort(pairs.begin(), pairs.end(), [](const pair<string,int> &a, const pair<string,int> &b){
        return a.second > b.second;
    });

    printf("*-----------Top 10 words-----------*\n");
    for(int i=0;i<10;++i){
        printf("%s: %d\n", pairs[i].first.c_str(), pairs[i].second);
    }
    printf("*---------------------------------*\n");
	std::cout << "Finished Parsing War and Peace" << endl;

	auto end = steady_clock::now();
    std::cout << "Parsing took "
              << duration_cast<milliseconds>(end - start).count()
              << "ms.\n";
    std::cout << "Found a total of " << nombre_lu << " words." << endl;
   vector<string> words = {"war", "peace", "love", "hate", "death", "life","toto"};
    for(auto &word : words){
        int *occurences = map.get(word);
        if(occurences){
            std::cout << "Found " << *occurences << " occurences of '" << word << "'." << endl;
        }
        else{
            std::cout << "Found 0 occurences of '" << word << "'." << endl;
        }
    }
	
    
	


    return 0;
}