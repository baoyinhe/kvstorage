#ifndef REALKVSTORAGE_H_
#define REALKVSTORAGE_H_

#include <memory>

#include "kvstorage.h"

class sqlite3;
class RealKVStorage: public KVStorage {
public:
  RealKVStorage(const std::string &filename);
  ~RealKVStorage(){};

  void insert(const std::string& key, const std::string& value) override;
  void insert(const std::string& key, int value) override;
  void insert(const std::string& key, double value) override;
  void insert(const std::string& key, const void* data, size_t size) override;

  void update(const std::string& key, const std::string& value) override;
  void update(const std::string& key, int value) override;
  void update(const std::string& key, double value) override;
  void update(const std::string& key, const void* data, size_t size) override;

  void remove(const std::string& key) override;

  std::vector<std::string> getAllKeys() const override;
  std::string get(const std::string &key) const override;
  std::string getString(const std::string& key) const override;
  int getInt(const std::string& key) const override;
  double getDouble(const std::string& key) const override;
  void* getData(const std::string& key, size_t& size) const override;

  bool exist(const std::string &key) const override;
  void clear() override;
  int count() const override;

  struct dbDeleter {
    void operator()(sqlite3* sqliteDB);
  };
private:
  static const std::string DEFAULT_TABLE;
  static const std::string MEMORY_DB;

  std::string                         filename_;   ///< UTF-8 filename used to open the database
  std::unique_ptr<sqlite3, dbDeleter> db_;         ///< Pointer to SQLite Database Connection Handle

  void dbInit();
  sqlite3* getHandle() const noexcept {
    return db_.get();
  }
};

#endif // REALKVSTORAGE_H_