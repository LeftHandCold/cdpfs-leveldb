//
// Created by sjw on 10/04/2018.
//

#include "db/table_cache.h"

#include "db/filename.h"
#include "leveldb/env.h"
#include "leveldb/table.h"
#include "util/coding.h"

namespace leveldb {

struct TableAndFile {
    RandomAccessFile* file;
    Table* table;
};

static void DeleteEntry(const Slice& key, void* value) {
    TableAndFile* tf = reinterpret_cast<TableAndFile*>(value);
    delete tf->table;
    delete tf->file;
    delete tf;
}

static void UnrefEntry(void* arg1, void* arg2) {
    Cache* cache = reinterpret_cast<Cache*>(arg1);
    Cache::Handle* h = reinterpret_cast<Cache::Handle*>(arg2);
    cache->Release(h);
}

TableCache::TableCache(const std::string& dbname,
                       const Options& options,
                       int entries)
        : env_(options.env),
          dbname_(dbname),
          options_(options),
          cache_(NULL) {
}

TableCache::~TableCache() {
    delete cache_;
}

Status TableCache::FindTable(uint64_t file_number, uint64_t file_size,
                             Cache::Handle** handle) {
    Status s;
    return s;
}

Iterator* TableCache::NewIterator(const ReadOptions& options,
                                  uint64_t file_number,
                                  uint64_t file_size,
                                  Table** tableptr) {
    if (tableptr != NULL) {
        *tableptr = NULL;
    }

    Iterator* result = NULL;
    return result;
}

Status TableCache::Get(const ReadOptions& options,
                       uint64_t file_number,
                       uint64_t file_size,
                       const Slice& k,
                       void* arg,
                       void (*saver)(void*, const Slice&, const Slice&)) {
    Cache::Handle* handle = NULL;
    Status s = FindTable(file_number, file_size, &handle);
    return s;
}

void TableCache::Evict(uint64_t file_number) {
    char buf[sizeof(file_number)];
    EncodeFixed64(buf, file_number);
    cache_->Erase(Slice(buf, sizeof(buf)));
}

}  // namespace leveldb
