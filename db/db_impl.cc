//
// Created by sjw on 01/04/2018.
//

#include "db/db_impl.h"


#include "db/builder.h"

#include "db/filename.h"
#include "db/memtable.h"
#include "db/table_cache.h"
#include "db/version_set.h"

#include "db/write_batch_internal.h"
#include "util/coding.h"

namespace leveldb {

const int kNumNonTableCacheFiles = 10;

// Information kept for every waiting writer
struct DBImpl::Writer {
    Status status;
    WriteBatch* batch;
    bool sync;
    bool done;
    //port::CondVar cv;

    explicit Writer(port::Mutex* mu) { }
};


void DBImpl::MaybeScheduleCompaction() {

    if (background_compaction_scheduled_) {
        // Already scheduled
    } else {
        background_compaction_scheduled_ = true;
        BGWork(this);
    }
}


void DBImpl::BGWork(void* db) {
    reinterpret_cast<DBImpl*>(db)->BackgroundCall();
}

void DBImpl::BackgroundCall() {

    assert(background_compaction_scheduled_);

    BackgroundCompaction();

    background_compaction_scheduled_ = false;
}

void DBImpl::BackgroundCompaction() {
    if (imm_ != NULL) {
        CompactMemTable();
        return;
    }

    printf("imm_ is NULL\n");
}


void DBImpl::CompactMemTable() {

    Status s = WriteLevel0Table(imm_);
}

Status DBImpl::WriteLevel0Table(MemTable* mem) {

    FileMetaData meta;
    meta.number = versions_->NewFileNumber();
    Iterator* iter = mem->NewIterator();
    Log(options_.info_log, "Level-0 table #%llu: started",
        (unsigned long long) meta.number);

    Status s;

    s = BuildTable(dbname_, env_, options_, table_cache_, iter, &meta);
}

Status DBImpl::MakeRoomForWrite(bool force) {
    bool allow_delay = !force;
    Status s;
    while (true) {
        if (!force &&
            (mem_->ApproximateMemoryUsage() <= options_.write_buffer_size)) {
            // There is room in current memtable
            break;
        } else {

            uint64_t new_log_number = versions_->NewFileNumber();
            WritableFile* lfile = NULL;
            s = env_->NewWritableFile(LogFileName(dbname_, new_log_number), &lfile);
            if (!s.ok()) {
                // Avoid chewing through file number space in a tight loop.
                versions_->ReuseFileNumber(new_log_number);
                break;
            }
            delete log_;
            delete logfile_;
            logfile_ = lfile;
            logfile_number_ = new_log_number;
            log_ = new log::Writer(lfile);
            imm_ = mem_;
            mem_ = new MemTable(internal_comparator_);
            mem_->Ref();
            force = false;   // Do not force another compaction if have room
            MaybeScheduleCompaction();

        }
    }
    WritableFile* lfile = NULL;
    s = env_->NewWritableFile(LogFileName(dbname_, 8), &lfile);
    if (!s.ok()) {
        return s;
    }

    return s;
}

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
    MemTable* mem = mem_;
    mem->Ref();
    SequenceNumber snapshot;
    snapshot = 1;
    // First look in the memtable, then in the immutable memtable (if any).
    LookupKey lkey(key, snapshot);

    mem->Get(lkey, value, &s);
    mem->Unref();
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

Status DBImpl::Write(const WriteOptions& options, WriteBatch* my_batch)
{
    WriteBatchInternal::SetSequence(my_batch, 1);
    Status status;
    status = log_->AddRecord(WriteBatchInternal::Contents(my_batch));

    status = WriteBatchInternal::InsertInto(my_batch, mem_);

    return status;
}

static int TableCacheSize(const Options& sanitized_options) {
    // Reserve ten files or so for other uses and give the rest to TableCache.
    return sanitized_options.max_open_files - kNumNonTableCacheFiles;
}

DBImpl::DBImpl(const Options& raw_options, const std::string& dbname)
        : env_(raw_options.env),
          internal_comparator_(raw_options.comparator),
          internal_filter_policy_(raw_options.filter_policy),
          mem_(NULL),
          imm_(NULL),
          logfile_(NULL),
          logfile_number_(0),
          log_(NULL),
          owns_info_log_(options_.info_log != raw_options.info_log),
          owns_cache_(options_.block_cache != raw_options.block_cache),
          dbname_(dbname),
          background_compaction_scheduled_(false),
          table_cache_(new TableCache(dbname_, options_, TableCacheSize(options_))),
          versions_(new VersionSet(dbname_, &options_, table_cache_,
                                        &internal_comparator_)) {
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

    {
        log::Writer log(file);
        std::string record;
        s = log.AddRecord(record);
        if (s.ok()) {
            s = file->Close();
        }
    }

    delete file;
    if (s.ok()) {
        // Make "CURRENT" file that points to the new manifest file.
        s = SetCurrentFile(env_, dbname_, 1);
    } else {
        env_->DeleteFile(manifest);
    }

    mem_ = new MemTable(internal_comparator_);
    mem_->Ref();
    return s;
}

// Default implementations of convenience methods that subclasses of DB
// can call if they wish
Status DB::Put(const WriteOptions& opt, const Slice& key, const Slice& value) {
    WriteBatch batch;
    batch.Put(key, value);
    return Write(opt, &batch);
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
    Status s = impl->NewDB();
    if (s.ok() && impl->mem_ != NULL) {
        WritableFile* lfile;
        s = options.env->NewWritableFile(LogFileName(dbname, 00005),
                                        &lfile);
        if (s.ok()) {
            impl->logfile_ = lfile;
            impl->logfile_number_ = 00005;
            impl->log_ = new log::Writer(lfile);
        }
    }

    if (s.ok()) {
        *dbptr = impl;
    }
    return s;
}

} // namespace leveldb
