//
// Created by sjw on 01/04/2018.
//

#include "db/db_impl.h"

#include "db/filename.h"

namespace leveldb {

bool DBImpl::GetProperty(const Slice& property, std::string* value) {

}
void DBImpl::GetApproximateSizes(const Range* range, int n, uint64_t* sizes) {

}
void DBImpl::CompactRange(const Slice* begin, const Slice* end) {

}

Status DBImpl::Get(const ReadOptions& options,
           const Slice& key,
           std::string* value) {
    Status s;
    return s;
}

Iterator* DBImpl::NewIterator(const ReadOptions& options) {
    return NULL;
}

// Convenience methods
Status DBImpl::Put(const WriteOptions& o, const Slice& key, const Slice& val) {
    return DB::Put(o, key, val);
}

Status DBImpl::Delete(const WriteOptions& options, const Slice& key) {
    return DB::Delete(options, key);
}

Status DBImpl::Write(const WriteOptions& options, WriteBatch* updates)
{
    Status s;
    return s;
}

DBImpl::DBImpl(const Options& raw_options, const std::string& dbname)
        : env_(raw_options.env),
          owns_info_log_(options_.info_log != raw_options.info_log),
          owns_cache_(options_.block_cache != raw_options.block_cache),
          dbname_(dbname) {
    printf("this is test\n");
}

DBImpl::~DBImpl() {
    if (owns_info_log_) {
        delete options_.info_log;
    }
    if (owns_cache_) {
        delete options_.block_cache;
    }
}

Status DBImpl::NewDB() {

    env_->CreateDir(dbname_);

    const std::string manifest = DescriptorFileName(dbname_, 1);
    WritableFile* file;
    Status s = env_->NewWritableFile(manifest, &file);
    if (!s.ok()) {
        return s;
    }

    delete file;
    if (s.ok()) {
        // Make "CURRENT" file that points to the new manifest file.
        s = SetCurrentFile(env_, dbname_, 1);
    } else {
        env_->DeleteFile(manifest);
    }
    return s;
}

// Default implementations of convenience methods that subclasses of DB
// can call if they wish
Status DB::Put(const WriteOptions& opt, const Slice& key, const Slice& value) {
    Status s;
    return s;
}

Status DB::Delete(const WriteOptions& opt, const Slice& key) {
    Status s;
    return s;
}

DB::~DB() { }

Status DB::Open(const Options& options, const std::string& dbname,
                DB** dbptr) {
    *dbptr = NULL;

    DBImpl* impl = new DBImpl(options, dbname);
    impl->NewDB();
    Status s;
    return s;
}

} // namespace leveldb
