#ifndef hm_h
#define hm_h

#include <vector>
#include <list>
#include <string>

template <typename T>
class HashMap
{
public:
 HashMap(double max_load = 0.75):myHash(10){
    if(max_load >= 0.75 ||  max_load <= 0) {
        loadFactor = 0.75;
    }else{
        loadFactor = max_load;
    }
    bracketNum = 10;
    numEntries = 0;
    /*
    for(int i = 0; i< bracketNum; i++){
        myHash[i] =  nullptr;
    }
    */

    }// constructor
 ~HashMap(){
    
    for(int i = 0; i< bracketNum; i++){
        if(!myHash[i].empty()) {
            /*
            std::list<*T> iterator::it  = myHash[i].begin();
            while(it!=myHash[i].end()){
                delete *it;
                it ++ ;
            }
            */
           myHash[i].clear();
        }
    }
    
 } // destructor; deletes all of the items in the hashmap

 int size() const{
    return numEntries;
 } // return the number of associations in the hashmap

 // The insert method associates one item (key) with another (value).
 // If no association currently exists with that key, this method inserts
 // a new association into the hashmap with that key/value pair. If there is
 // already an association with that key in the hashmap, then the item
 // associated with that key is replaced by the second parameter (value).
 // Thus, the hashmap must contain no duplicate keys.
 void insert(const std::string& key, const T& value){
    size_t h = std::hash<std::string>()(key);
    int  index = h % bracketNum;
    typename std::list<association>::iterator it  = myHash[index].begin();
            while(it!=myHash[index].end()){
                if(key==(*it).key){
                    (*it).val = value;
                    return;
                }
                it++;
            }
    myHash[index].push_back(association(key, value)); //May be wrong
        numEntries++;

    double Bcount = 0.0;
    for(int i=0;i<bracketNum;i++){
        if(!myHash[i].empty()){
            Bcount +=1;
        }
    }
    if(Bcount/bracketNum >= loadFactor){
        bracketNum *= 2;
        std::vector<std::list<association>> myHashNew(bracketNum);
        for(int j=0;j<myHash.size();j++){ //N
            if(!myHash[j].empty()){
                typename std::list<association>::iterator it  = myHash[j].begin();
                while(it!=myHash[j].end()){
                    size_t newh = std::hash<std::string>()(it->key);
                    int newInex = newh % bracketNum;
                myHashNew[newInex].push_back((*it));
                it++;
                }
            }
            myHash[j].clear();
        }
        //std::vector<std::list<association>> myHashNew(myHash.begin(),myHash.end());
        /*
        for(int j=0;j<myHash.size();j++){ //N
            if(!myHash[j].empty()){
                typename std::list<association>::iterator it  = myHash[j].begin();
                while(it!=myHash[j].end()){
                myHashNew[j].push_back(myHash[j].front());
                it++;
                }
            }
            myHash[j].clear();
        }
        */

        /*
         for(int j=0;j<myHash.size();j++){ 
            myHash[j].clear();
         }

         */
        myHash.resize(bracketNum);
        //numEntries = 0;
        //int newIndex = h%bracketNum;
        myHash = myHashNew;
        /*
        for(int i=0;i<bracketNum/2;i++){ // do I need to redistribute here?
            if(!myHashNew[i].empty()){
                typename  std::list<association>::iterator  it  = myHashNew[i].begin();
                while(it!=myHashNew[i].end()){
                    insert((*it).key,(*it).val);
                    it ++;
                }
            }
        }
        */
    }

 }
 // Defines the bracket operator for HashMap, so you can use your map like this:
 // your_map["david"] = 2.99;
 // If the key does not exist in the hashmap, this will create a new entry in
 // the hashmap and map it to the default value of type T. Then it will return a
 // reference to the newly created value in the map.
 T& operator[](const std::string& key){
    if(find(key)==nullptr){
        size_t h = std::hash<std::string>()(key);
        int  index = h % bracketNum;
        T defaultVal = T();
        myHash[index].push_back(association(key,defaultVal));
        numEntries++;
    }
    return *find(key);
 }

  const T* find(const std::string& key) const{
    size_t h = std::hash<std::string>()(key);
    int  index = h % bracketNum;
    if(myHash[index].empty())return nullptr;
    for(auto it = myHash[index].begin();it != myHash[index].end();it++){
        if ((*it).key == key){
            return &((*it).val);
        }
    }
    return nullptr;
  }
 // If no association exists with the given key, return nullptr; otherwise,
 // return a pointer to the value associated with that key. This pointer can be
 // used to examine that value or modify it directly within the map.
 T* find(const std::string& key) {
 const auto& hm = *this;
 return const_cast<T*>(hm.find(key));
 }


 private:
  struct association{
    association(std::string key_, T val_):key(key_),val(val_) {
        //key = key_; 
        //val = val_;
    }
    std::string  key;
    T val;
 };
 int bracketNum;
 double loadFactor;
 std::vector<std::list<association>> myHash;
 int  numEntries;



}; 

#endif