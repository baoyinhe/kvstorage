#ifndef KVSTORAGE_H_
#define KVSTORAGE_H_

#include <string>
#include <mutex>

#include "kvstorage_export.h"

class sqlite3;
class KVSTORAGE_API KVStorage {
private:
  static const std::string DEFAULT_TABLE;
  static const std::string MEMORY_DB;

  std::string filename_;
  sqlite3* db_;
  std::mutex lock_;

  void dbInit();

 public:
  KVStorage(const std::string &filename);
  ~KVStorage();
  void insert(const std::string& key, const std::string& value);
  void update(const std::string& key, const std::string& value);
  void remove(const std::string& key);
  std::string get(const std::string& key); // TODO支持各种返回类型
  void clear();
  int count();
};

#endif // KVSTORAGE_H_