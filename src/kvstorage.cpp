#include "kvstorage.h"
#include <sqlite3.h>

const std::string KVStorage::DEFAULT_TABLE = "KVS_DEFAULT_TABLE";
const std::string KVStorage::MEMORY_DB = ":memory:";

KVStorage::KVStorage(const std::string& filename): filename_(filename), db_(nullptr) {
  if (filename.empty() || filename == MEMORY_DB) {
    filename_ = MEMORY_DB;
  }
  else {
    filename_ = filename;
  }
  dbInit();
};

KVStorage::~KVStorage() {
  sqlite3_close(db_);
}

void KVStorage::insert(const std::string& key, const std::string& value) {
  std::lock_guard<std::mutex> guard(lock_);
  sqlite3_stmt* stmt;
  std::string sql = "insert into " + DEFAULT_TABLE + " (k, v) VALUES (?1, ?2)";
  sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, NULL);
  sqlite3_bind_text(stmt, 1, key.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 2, value.c_str(), -1, SQLITE_STATIC); // TODO 其他类型
  sqlite3_step(stmt);
  sqlite3_finalize(stmt);
}

void KVStorage::update(const std::string& key, const std::string& value) {
  std::lock_guard<std::mutex> guard(lock_);
  sqlite3_stmt* stmt;
  std::string sql = "update " + DEFAULT_TABLE + " SET v = ?1 WHERE k = ?2";
  sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, NULL);
  sqlite3_bind_text(stmt, 1, value.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 2, key.c_str(), -1, SQLITE_STATIC); // TODO 其他类型
  sqlite3_step(stmt);
  sqlite3_finalize(stmt);
}

void KVStorage::remove(const std::string& key) {
  std::lock_guard<std::mutex> guard(lock_);
  sqlite3_stmt* stmt;
  std::string sql = "DELETE FROM " + DEFAULT_TABLE + " WHERE k = ?1";
  sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, NULL);
  sqlite3_bind_text(stmt, 1, key.c_str(), -1, SQLITE_STATIC);
  sqlite3_step(stmt);
  sqlite3_finalize(stmt);
}

std::string KVStorage::get(const std::string& key) {
  std::lock_guard<std::mutex> guard(lock_);
  sqlite3_stmt* stmt;
  std::string sql = "SELECT v FROM " + DEFAULT_TABLE + " WHERE k = ?1";
  sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, NULL);
  sqlite3_bind_text(stmt, 1, key.c_str(), -1, SQLITE_STATIC);
  sqlite3_step(stmt);
  std::string value;
  if (sqlite3_column_text(stmt, 0) != nullptr) {
      value = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
  }
  sqlite3_finalize(stmt);
  return value;
}

void KVStorage::clear() {
  std::lock_guard<std::mutex> guard(lock_);
  sqlite3_stmt* stmt;
  std::string sql = "DELETE FROM " + DEFAULT_TABLE;
  sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, NULL);
  sqlite3_step(stmt);
  sqlite3_finalize(stmt);
}

int KVStorage::count() {
  std::lock_guard<std::mutex> guard(lock_);
  sqlite3_stmt* stmt;
  std::string sql =   "SELECT COUNT(*) FROM " + DEFAULT_TABLE;
  sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, NULL);
  sqlite3_step(stmt);
  int count = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);
  return count;
}

void KVStorage::dbInit() {
  std::string newTable = "CREATE TABLE IF NOT EXISTS " + DEFAULT_TABLE + " (k TEXT PRIMARY KEY NOT NULL, \
                                                                            v TEXT              NULL)";
  sqlite3_open(filename_.c_str(), &db_);
  sqlite3_exec(db_, newTable.c_str(), NULL, 0, NULL);
}
