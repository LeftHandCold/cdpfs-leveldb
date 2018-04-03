//
// Created by sjw on 01/04/2018.
//

#ifndef CDPFS_LEVELDB_DB_DB_IMPL_H
#define CDPFS_LEVELDB_DB_DB_IMPL_H

#include <deque>
#include <set>
#include "db/dbformat.h"
#include "db/log_writer.h"
#include "leveldb/db.h"
#include "leveldb/env.h"
#include "port/port.h"
#include "port/thread_annotations.h"

namespace leveldb {

class MemTable;
class TableCache;
class Version;
class VersionEdit;
class VersionSet;

class DBImpl : public DB {
public:
    DBImpl(const Options& options, const std::string& dbname);
    virtual ~DBImpl();

    // Implementations of the DB interface
    virtual Status Put(const WriteOptions&, const Slice& key, const Slice& value);
    virtual Status Delete(const WriteOptions&, const Slice& key);
    virtual Status Write(const WriteOptions& options, WriteBatch* updates);
    virtual Status Get(const ReadOptions& options,
                       const Slice& key,
                       std::string* value);
    virtual Iterator* NewIterator(const ReadOptions&);
    virtual bool GetProperty(const Slice& property, std::string* value);
    virtual void GetApproximateSizes(const Range* range, int n, uint64_t* sizes);
    virtual void CompactRange(const Slice* begin, const Slice* end);

private:
    friend class DB;
    struct CompactionState;
    struct Writer;


    Status NewDB();

    // Recover the descriptor from persistent storage.  May do a significant
    // amount of work to recover recently logged updates.  Any changes to
    // be made to the descriptor are added to *edit.
    Status Recover(VersionEdit* edit, bool* save_manifest);

    Status MakeRoomForWrite(bool force /* compact even if there is room? */);


    // Constant after construction
    Env* const env_;
    const InternalKeyComparator internal_comparator_;
    const InternalFilterPolicy internal_filter_policy_;
    const Options options_;  // options_.comparator == &internal_comparator_
    const bool owns_info_log_;
    const bool owns_cache_;
    const std::string dbname_;

    MemTable* mem_;
    MemTable* imm_;
    WritableFile* logfile_;
    uint64_t logfile_number_;
    log::Writer* log_;


    // No copying allowed
    DBImpl(const DBImpl&);
    void operator=(const DBImpl&);

};

}  // namespace leveldb


#endif //CDPFS_LEVELDB_DB_DB_IMPL_H
