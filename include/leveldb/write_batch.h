//
// Created by sjw on 01/04/2018.
//
// WriteBatch holds a collection of updates to apply atomically to a DB.
//
// The updates are applied in the order in which they are added
// to the WriteBatch.  For example, the value of "key" will be "v3"
// after the following batch is written:
//
//    batch.Put("key", "v1");
//    batch.Delete("key");
//    batch.Put("key", "v2");
//    batch.Put("key", "v3");
//
// Multiple threads can invoke const methods on a WriteBatch without
// external synchronization, but if any of the threads may call a
// non-const method, all threads accessing the same WriteBatch must use
// external synchronization.

#ifndef CDPFS_LEVELDB_INCLUDE_WRITE_BATCH_H
#define CDPFS_LEVELDB_INCLUDE_WRITE_BATCH_H

#include <string>
#include "leveldb/export.h"
#include "leveldb/status.h"

namespace leveldb {

class Slice;

class LEVELDB_EXPORT WriteBatch {
public:
    WriteBatch();
    ~WriteBatch();

    // Store the mapping "key->value" in the database.
    void Put(const Slice& key, const Slice& value);

    // If the database contains a mapping for "key", erase it.  Else do nothing.
    void Delete(const Slice& key);

    // Clear all updates buffered in this batch.
    void Clear();

    // The size of the database changes caused by this batch.
    //
    // This number is tied to implementation details, and may change across
    // releases. It is intended for LevelDB usage metrics.
    size_t ApproximateSize();

    // Support for iterating over the contents of a batch.
    class Handler {
    public:
        virtual ~Handler();
        virtual void Put(const Slice& key, const Slice& value) = 0;
        virtual void Delete(const Slice& key) = 0;
    };
    Status Iterate(Handler* handler) const;

private:
    friend class WriteBatchInternal;

    std::string rep_;  // See comment in write_batch.cc for the format of rep_

    // Intentionally copyable
};

}  // namespace leveldb

#endif //CDPFS_LEVELDB_INCLUDE_WRITE_BATCH_H
