#include <iostream>
#include <fstream>
#include <regex>
#include <chrono>
#include <forward_list>

template<typename K, typename V>
class Entry{
    public:
        const K key;
        V value;
        Entry(K key, V value): key(key), value(value){}
};


template<typename K, typename V>
class HashMap{
    std::vector<std::forward_list<Entry<K,V>>> buckets;
    public:
        HashMap(size_t size){
            buckets.reserve(size);
            for(int i=0;i<size;++i){
                buckets.push_back(std::forward_list<Entry<K,V>>());
        }
        }
        void insert(K key, V value){
            int index = hash(key);
            V *existing = get(key);
            if(existing){
                (*existing)++;
            }
            else{
                Entry entry(key, value);
                buckets[index].push_front(entry);                
            }
        }
        V* get(const K key){
            int index = hash(key);
            for(auto &entry : buckets[index]){
                if(entry.key == key){
                    return &entry.value;
                }
            }
            return nullptr;
        }
    const std::vector<std::forward_list<Entry<K,V>>> &getBuckets(){
        return buckets;
    }
    private:
        int hash(K key){
            return std::hash<K>()(key) % buckets.size();
        }
};