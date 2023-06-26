#ifndef KVSTORAGE_H_
#define KVSTORAGE_H_

#include <string>
#include <vector>

#include <kvstorage_export.h>

class KVSTORAGE_API KVStorage {
 public:
  KVStorage(){};
  virtual ~KVStorage(){};

  virtual void insert(const std::string& key, const std::string& value) = 0;
  virtual void insert(const std::string& key, int value) = 0;
  virtual void insert(const std::string& key, double value) = 0;
  virtual void insert(const std::string& key, const void* data, size_t size) = 0;

  virtual void update(const std::string& key, const std::string& value) = 0;
  virtual void update(const std::string& key, int value) = 0;
  virtual void update(const std::string& key, double value) = 0;
  virtual void update(const std::string& key, const void* data, size_t size) = 0;

  virtual void remove(const std::string& key) = 0;

  virtual std::vector<std::string> getAllKeys() const = 0;
  virtual std::string get(const std::string& key) const = 0;
  virtual std::string getString(const std::string& key) const = 0;
  virtual int getInt(const std::string& key) const = 0;
  virtual double getDouble(const std::string& key) const = 0;
  virtual void *getData(const std::string &key, size_t &size) const = 0;

  virtual bool exist(const std::string &key) const = 0;
  virtual void clear() = 0;
  virtual int count() const = 0;
};

KVSTORAGE_API KVStorage* create(const std::string &filename="");

#endif // KVSTORAGE_H_