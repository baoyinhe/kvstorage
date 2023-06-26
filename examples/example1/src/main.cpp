#include <iostream>
#include <kvstorage.h>
#include <vector>
 

int main(int argc, char **argv){
    KVStorage *db = create("example.db");
    db->insert("key1", "v1");
    db->insert("key2", 2);
    db->insert("key3", 3.1);

    std::vector<std::string> vec = db->getAllKeys();
    for (size_t i = 0; i < vec.size(); i++) {
        std::cout << "key=" << vec[i] << " value=" << db->get(vec[i]) << std::endl;
    }

    db->update("key1", 888);
    std::cout << "update key1=" << db->get("key1") << std::endl;
    std::cout << "before remove key2, count=" << db->count() << std::endl;
    db->remove("key2");
    std::cout << "after remove key2, count=" << db->count() << std::endl;
    std::cout << "key1 exist: "<< db->exist("key1") << std::endl;
    std::cout << "key2 exist: " << db->exist("key2") << std::endl;
    std::cout << "key2=" << db->get("key2") << std::endl;
    db->clear();
    std::cout << "after clear, count=" << db->count() << std::endl;

    return 0;
}