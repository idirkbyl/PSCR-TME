#include "hashmap.hpp"
#include <iostream>
#include <fstream>
#include <regex>
#include <chrono>
#include <forward_list>
#include <cassert>


template<typename iterator>
size_t count(iterator begin, iterator end){
    size_t res = 0;
    for(auto it=begin;it!=end;++it){
        res++;
    }
    return res;
}

template<typename iterator,typename T>
size_t count_if_equal(iterator begin, iterator end,const T & val){
    size_t res=0;
    for(auto it=begin; it!=end;++it){
        if(*it==val){
            res++;
        }
    }
    return res;
}

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
    vector<string> words;

	// une regex qui reconnait les caractères anormaux (négation des lettres)
	regex re( R"([^a-zA-Z])");
	while (input >> word) {
		// élimine la ponctuation et les caractères spéciaux
		word = regex_replace ( word, re, "");

		// passe en lowercase

		transform(word.begin(),word.end(),word.begin(),::tolower);

		// word est maintenant "tout propre"


        map.insert(word, 1);
        words.push_back(word);
        
		if (nombre_lu % 100 == 0)
			// on affiche un mot "propre" sur 100
			std::cout << nombre_lu << ": "<< word << endl;

		nombre_lu++;
	}
	input.close();


	std::cout << "Finished Parsing War and Peace" << endl;
	auto end = steady_clock::now();
    std::cout << "Parsing took "
              << duration_cast<milliseconds>(end - start).count()
              << "ms.\n";
        
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


    std::cout << "Found a total of " << nombre_lu << " words." << endl;
    /*Test des fonctions count et count_if_equal*/
    assert(count(words.begin(), words.end()) == nombre_lu);
    assert(count_if_equal(words.begin(), words.end(), "toto") == 0);
   vector<string> words2 = {"war", "peace", "love", "hate", "death", "life","toto"};
    for(auto &word : words2){
        int *occurences = map.get(word);
        assert(count_if_equal(words.begin(), words.end(), word) == (occurences ? *occurences : 0));
        if(occurences){
            std::cout << "Found " << *occurences << " occurences of '" << word << "'." << endl;
        }
        else{
            std::cout << "Found 0 occurences of '" << word << "'." << endl;
        }
    }
	/*Recopie des entrées grace a l'itérateur*/
    vector<pair<string,int>> pairs2;
    for(const auto &entry :map){
        pairs2.push_back(make_pair(entry.key,entry.value));
    }
    
    sort(pairs2.begin(),pairs2.end(),[](const pair<string,int> &a, const pair<string,int> &b){
        return a.second > b.second;
    });

    printf("*-----------Top 10 words (Using vector created with HashMap iterator)-----------*\n");
    for(int i=0;i<10;++i){
        printf("%s: %d\n", pairs2[i].first.c_str(), pairs2[i].second);
    }
    printf("*---------------------------------*\n");
	


    return 0;
}