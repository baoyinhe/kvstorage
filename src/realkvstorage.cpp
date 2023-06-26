#include <realkvstorage.h>
#include <sqlite3.h>
#include <future>

const std::string RealKVStorage::DEFAULT_TABLE = "KVS_DEFAULT_TABLE";
const std::string RealKVStorage::MEMORY_DB = ":memory:";

RealKVStorage::RealKVStorage(const std::string& filename): filename_(filename) {
  if (filename.empty() || filename == MEMORY_DB) {
    filename_ = MEMORY_DB;
  } else {
    filename_ = filename;
  }
  dbInit();
};

void RealKVStorage::insert(const std::string& key, const std::string &value) {
  std::string sql = "INSERT INTO " + DEFAULT_TABLE + " (k, v) VALUES ('" + key + "', '" + value + "')";
  auto task = std::async(std::launch::async, [this, sql]() {
    const int ret = sqlite3_exec(getHandle(), sql.c_str(), nullptr, nullptr, nullptr);
    if (SQLITE_OK != ret) {
      throw "INSERT ERROR";  // TODO 子线程触发的异常如何捕获
    }
  });
}

void RealKVStorage::insert(const std::string& key, int value) {
  std::string val = std::to_string(value);
  insert(key, val);
}

void RealKVStorage::insert(const std::string& key, double value) {
  std::string val = std::to_string(value);
  insert(key, val);
}

void RealKVStorage::insert(const std::string& key, const void* data, size_t size) {
  std::string value(reinterpret_cast<const char*>(data), size);
  insert(key, value);
}

void RealKVStorage::update(const std::string& key, const std::string& value) {
  std::string sql = "UPDATE " + DEFAULT_TABLE + " SET v = '" + value + "' WHERE k = '" + key + "'";
  auto task = std::async(std::launch::async, [this, sql]() {
    sqlite3_exec(getHandle(), sql.c_str(), nullptr, nullptr, nullptr);
  });
}

void RealKVStorage::update(const std::string& key, int value) {
  std::string val = std::to_string(value);
  update(key, val);
}

void RealKVStorage::update(const std::string& key, double value) {
  std::string val = std::to_string(value);
  update(key, val);
}

void RealKVStorage::update(const std::string& key, const void* data, size_t size) {
  std::string value(reinterpret_cast<const char*>(data), size);
  update(key, value);
}

void RealKVStorage::remove(const std::string& key) {
  std::string sql = "DELETE FROM " + DEFAULT_TABLE + " WHERE k = '" + key + "'";
  auto task = std::async(std::launch::async, [this, sql]() {
    sqlite3_exec(getHandle(), sql.c_str(), nullptr, nullptr, nullptr);
  });
}

std::vector<std::string> RealKVStorage::getAllKeys() const {
  sqlite3_stmt* stmt;
  std::string sql = "SELECT k FROM " + DEFAULT_TABLE;
  sqlite3_prepare_v2(getHandle(), sql.c_str(), -1, &stmt, nullptr);

  std::vector<std::string> keys;

  while (sqlite3_step(stmt) == SQLITE_ROW) {
    keys.emplace_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
  }

  sqlite3_finalize(stmt);

  return keys;
}

std::string RealKVStorage::get(const std::string& key) const {
  sqlite3_stmt* stmt;
  std::string sql = "SELECT v FROM " + DEFAULT_TABLE + " WHERE k = '" + key + "'";
  sqlite3_prepare_v2(getHandle(), sql.c_str(), -1, &stmt, nullptr);
  const int ret = sqlite3_step(stmt);
  std::string value;
  if (SQLITE_ROW == ret) {
    value = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
  }
  sqlite3_finalize(stmt);
  return value;
}

std::string RealKVStorage::getString(const std::string& key) const {
  return get(key);
}

int RealKVStorage::getInt(const std::string& key) const {
  std::string value = get(key);
  return std::stoi(value);
}

double RealKVStorage::getDouble(const std::string& key) const {
  std::string value = get(key);
  return std::stod(value);
}

void* RealKVStorage::getData(const std::string& key, size_t& size) const {
  std::string value = get(key);
  size = value.size();
  return const_cast<char*>(value.data());
}

bool RealKVStorage::exist(const std::string &key) const {
  sqlite3_stmt* stmt;
  std::string sql = "SELECT COUNT(*) FROM " + DEFAULT_TABLE + " WHERE k = '" + key + "'";
  sqlite3_prepare_v2(getHandle(), sql.c_str(), -1, &stmt, nullptr);
  sqlite3_step(stmt);
  int count = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);
  return count > 0;
}

void RealKVStorage::clear() {
  std::string sql = "DELETE FROM " + DEFAULT_TABLE;
  sqlite3_exec(getHandle(), sql.c_str(), nullptr, nullptr, nullptr);
}

int RealKVStorage::count() const {
  sqlite3_stmt* stmt;
  std::string sql = "SELECT COUNT(*) FROM " + DEFAULT_TABLE;
  sqlite3_prepare_v2(getHandle(), sql.c_str(), -1, &stmt, nullptr);
  sqlite3_step(stmt);
  int count = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);
  return count;
}

void RealKVStorage::dbDeleter::operator()(sqlite3* sqliteDB) {
  sqlite3_close(sqliteDB);
}


void RealKVStorage::dbInit() {
  sqlite3* handle;
  int ret = sqlite3_open(filename_.c_str(), &handle);
  if (SQLITE_OK != ret) {
    throw "OPEN ERROR";
  }
  db_.reset(handle);
  sqlite3 *pdb = getHandle();
  std::string newTable = "CREATE TABLE IF NOT EXISTS " + DEFAULT_TABLE + " (k TEXT PRIMARY KEY NOT NULL, \
                                                                            v TEXT              NULL)";
  ret = sqlite3_exec(getHandle(), newTable.c_str(), nullptr, 0, nullptr);
  if (SQLITE_OK != ret) {
    throw "CREATE TABLE ERROR";
  }
}


KVStorage* create(const std::string &filename) {
  return new RealKVStorage(filename);
}

