/*
#include "HashMap.h"
#include <iostream>
#include <cassert>
#include <iostream>
#include <string>
//#include "geopoint.h"
using  namespace std;

void testInsertAndFind() {
    HashMap<int> map;
    map.insert("key1", 10);
    map.insert("key2", 20);

    assert(*map.find("key1") == 10);
    assert(*map.find("key2") == 20);
    assert(map.find("key3") == nullptr);
}

void testSize() {
    HashMap<double> map;
    assert(map.size() == 0);

    map.insert("gpa1", 3.5);
    assert(map.size() == 1);

    map.insert("gpa2", 4.0);
    assert(map.size() == 2);
}

void testOperatorBracket() {
    HashMap<std::string> map;
    map["name1"] = "Alice";
    map["name2"] = "Bob";

    assert(map.find("name1") != nullptr && *map.find("name1") == "Alice");
    assert(map.find("name2") != nullptr && *map.find("name2") == "Bob");
    assert(map["name3"] == "");  // Default value for std::string
}

void testRehash() {
    HashMap<int> map(0.5);  // Set max_load to 0.5 to trigger rehashing sooner
    for (int i = 0; i < 1000; ++i) {
        map.insert("key" + std::to_string(i), i);
    }
    assert(map.size() == 1000);
    // Add more checks to verify that rehashing occurred and the elements are still accessible
}

void testDuplicateKeys() {
    HashMap<std::string> map;
    map.insert("key", "value1");
    map.insert("key", "value2");

    assert(*map.find("key") == "value2");
    assert(map.size() == 1);
}

void testLoadFactorAndRehash() {
    HashMap<int> map(0.3);  // Low load factor to trigger rehashing early
    assert(map.size() == 0);

    // Insert enough elements to trigger rehashing
    for (int i = 0; i < 4; ++i) {
        map.insert("key" + std::to_string(i), i);
    }

    // Check that all elements are still accessible after rehashing
    for (int i = 0; i < 4; ++i) {
        assert(*map.find("key" + std::to_string(i)) == i);
    }
}

void testUpdateValue() {
    HashMap<float> map;
    map.insert("pi", 3.14f);
    map.insert("pi", 3.1415f);

    assert(*map.find("pi") == 3.1415f);
}

void testOperatorBracketForUpdating() {
    HashMap<int> map;
    map["counter"] = 1;
    map["counter"] = 2;

    assert(map["counter"] == 2);
}

void testRemovingEntries() {
    
}

void testClear() {
    HashMap<double> map;
}

void testLargeNumberOfInsertions() {
    HashMap<int> map;
    const int N = 1000;

    for (int i = 0; i < N; ++i) {
        map.insert("key" + std::to_string(i), i);
    }

    assert(map.size() == N);
    for (int i = 0; i < N; ++i) {
        assert(*map.find("key" + std::to_string(i)) == i);
    }
}

void testRehashingWithLargeData() {
    HashMap<int> map(0.2);  // Very low load factor to trigger frequent rehashing
    const int N = 1000;

    for (int i = 0; i < N; ++i) {
        map.insert("key" + std::to_string(i), i);
    }

    assert(map.size() == N);
    for (int i = 0; i < N; ++i) {
        assert(*map.find("key" + std::to_string(i)) == i);
    }
}

int main() {
    testInsertAndFind();
    testSize();
    testOperatorBracket();
    testRehash();
    testDuplicateKeys();
    testLoadFactorAndRehash();
    testUpdateValue();
    testOperatorBracketForUpdating();
    testRemovingEntries();
    testClear();
    testLargeNumberOfInsertions();
    testRehashingWithLargeData();

    std::cout << "All tests passed!" << std::endl;
    return 0;
}


*/

#include <iostream>
using namespace std;
template <typename K>
K max(K x, K y){
    if(x>y){
        return x;
    }else{
        return y;
    }
}

int main(){
    int a = 5;
    double b = 3;
    cout << max(a,b);
}