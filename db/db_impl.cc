//
// Created by sjw on 01/04/2018.
//

#include "db/db_impl.h"

#include "db/filename.h"
#include "db/memtable.h"
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

Status DBImpl::MakeRoomForWrite(bool force) {
    Status s;
    
    WritableFile* lfile = NULL;
    s = env_->NewWritableFile(LogFileName(dbname_, 8), &lfile);
    if (!s.ok()) {
        return s;
    }

    delete log_;
    delete logfile_;
    logfile_ = lfile;
    logfile_number_ = 8;
    log_ = new log::Writer(lfile);
    force = false;   // Do not force another compaction if have room

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
    //Status status = MakeRoomForWrite(my_batch == NULL);
    Status status;
    //if (status.ok() && my_batch == NULL) { //NULL batch is for compactions
        std::string key = "testkey";
        std::string val = "testval";
        std::string rep;
    rep.resize(12);
    int te = DecodeFixed32(rep.data() + 8);
    EncodeFixed32(&rep[8], te + 1);
    rep.push_back(static_cast<char>(kTypeValue));
    PutLengthPrefixedSlice(&rep, key);
    PutLengthPrefixedSlice(&rep, val);
    SequenceNumber seq = 1;
    EncodeFixed64(&rep[0], seq);
    seq = DecodeFixed32(rep.data() + 8);
    DecodeFixed32(rep.data() + 8);
        status = log_->AddRecord(Slice(rep));

    bool sync_error = false;
    if (status.ok() && options.sync) {
        status = logfile_->Sync();
        if (!status.ok()) {
            sync_error = true;
        }
    }

    Slice input(rep);
    input.remove_prefix(12);
    Slice k, value;
    int found = 0;
    while (!input.empty()) {
        found++;
        char tag = input[0];
        input.remove_prefix(1);

        if (GetLengthPrefixedSlice(&input, &k) &&
            GetLengthPrefixedSlice(&input, &value)) {

            SequenceNumber sequence_ = SequenceNumber(rep.data());
            mem_->Add(sequence_, kTypeValue, k, value);
            sequence_++;
        }
    }


    //}
    return status;
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
    return Write(opt, NULL);
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
