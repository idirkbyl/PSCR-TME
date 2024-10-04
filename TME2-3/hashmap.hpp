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
    typedef std::vector<std::forward_list<Entry<K,V>>> buckets_t;
    std::vector<std::forward_list<Entry<K,V>>> buckets;

    struct iterator{
        buckets_t &buckets;
        size_t bucketIndex;
        typedef std::forward_list<Entry<K,V>>::iterator list_it;
        list_it lit;

        iterator(buckets_t &buckets, size_t bucketIndex, list_it lit): buckets(buckets), bucketIndex(bucketIndex), lit(lit){}

        iterator &operator++(){
            ++lit;
            if(lit == buckets[bucketIndex].end()){
                ++bucketIndex;
                while(bucketIndex < buckets.size() && buckets[bucketIndex].empty()){
                    ++bucketIndex;
                }
                if(bucketIndex < buckets.size()){
                    lit = buckets[bucketIndex].begin();
                }
            }
            return *this;
        }

        Entry<K,V> &operator*(){
            return *lit;
        }

        bool operator!=(const iterator &other){
            return bucketIndex != other.bucketIndex || lit != other.lit;
        }
    };

    public:
        HashMap(size_t size){
            buckets.reserve(size);
            for(int i=0;i<size;++i){
                buckets.push_back(std::forward_list<Entry<K,V>>());
        }
        }

        HashMap (const HashMap &other){
            for(auto &bucket : other.getBuckets()){
                for(auto &entry : bucket){
                    insert(entry.key, entry.value);
                }
            }
        }

        HashMap & operator=(const HashMap &other){
            if(this!=other){
                for(auto &bucket : other.getBuckets()){
                    for(auto &entry : bucket){
                        insert(entry.key, entry.value);
                    }
                }
            }
            return *this;
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

        iterator begin(){
            size_t index=0;
            while(index<buckets.size() && buckets[index].empty()){
                index++;
            }
            if(index < buckets.size()){
                return iterator(buckets, index, buckets[index].begin());
            }
            return end();
        }

        iterator end(){
            return iterator(buckets, buckets.size(), std::forward_list<Entry<K,V>>().end());
        }

    const std::vector<std::forward_list<Entry<K,V>>> &getBuckets(){
        return buckets;
    }



    private:
        int hash(K key){
            return std::hash<K>()(key) % buckets.size();
        }
};