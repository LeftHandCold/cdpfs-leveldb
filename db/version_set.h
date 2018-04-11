//
// Created by sjw on 10/04/2018.
//

#ifndef CDPFS_LEVELDB_DB_VERSION_SET_H
#define CDPFS_LEVELDB_DB_VERSION_SET_H

#include <map>
#include <set>
#include <vector>
#include "db/dbformat.h"
#include "port/port.h"
#include "port/thread_annotations.h"

namespace leveldb {

struct FileMetaData {
    int refs;
    int allowed_seeks;          // Seeks allowed until compaction
    uint64_t number;
    uint64_t file_size;         // File size in bytes
    InternalKey smallest;       // Smallest internal key served by table
    InternalKey largest;        // Largest internal key served by table

    FileMetaData() : refs(0), allowed_seeks(1 << 30), file_size(0) { }
};

namespace log { class Writer; }

class Compaction;
class Iterator;
class MemTable;
class TableBuilder;
class TableCache;
class VersionSet;
class WritableFile;

class VersionSet {
public:
    VersionSet(const std::string &dbname,
               const Options *options,
               TableCache *table_cache,
               const InternalKeyComparator *);

    ~VersionSet();

    // Return the current manifest file number
    uint64_t ManifestFileNumber() const { return manifest_file_number_; }

    // Allocate and return a new file number
    uint64_t NewFileNumber() { return next_file_number_++; }

    // Arrange to reuse "file_number" unless a newer file number has
    // already been allocated.
    // REQUIRES: "file_number" was returned by a call to NewFileNumber().
    void ReuseFileNumber(uint64_t file_number) {
        if (next_file_number_ == file_number + 1) {
            next_file_number_ = file_number;
        }
    }

    // Return the number of Table files at the specified level.
    int NumLevelFiles(int level) const;

    // Return the combined file size of all files at the specified level.
    int64_t NumLevelBytes(int level) const;

    // Return the last sequence number.
    uint64_t LastSequence() const { return last_sequence_; }

    // Set the last sequence number to s.
    void SetLastSequence(uint64_t s) {
        assert(s >= last_sequence_);
        last_sequence_ = s;
    }

    // Mark the specified file number as used.
    void MarkFileNumberUsed(uint64_t number);

    // Return the current log file number.
    uint64_t LogNumber() const { return log_number_; }

    // Return the log file number for the log file that is currently
    // being compacted, or zero if there is no such log file.
    uint64_t PrevLogNumber() const { return prev_log_number_; }

    // Return the maximum overlapping data (in bytes) at next level for any
    // file at a level >= 1.
    int64_t MaxNextLevelOverlappingBytes();

    // Create an iterator that reads over the compaction inputs for "*c".
    // The caller should delete the iterator when no longer needed.
    Iterator *MakeInputIterator(Compaction *c);

    // Return a human-readable short (single-line) summary of the number
    // of files per level.  Uses *scratch as backing store.
    struct LevelSummaryStorage {
        char buffer[100];
    };

    const char *LevelSummary(LevelSummaryStorage *scratch) const;

private:
    class Builder;

    friend class Compaction;

    friend class Version;

    bool ReuseManifest(const std::string &dscname, const std::string &dscbase);

    void SetupOtherInputs(Compaction *c);

    // Save current contents to *log
    Status WriteSnapshot(log::Writer *log);


    Env *const env_;
    const std::string dbname_;
    const Options *const options_;
    TableCache *const table_cache_;
    const InternalKeyComparator icmp_;
    uint64_t next_file_number_;
    uint64_t manifest_file_number_;
    uint64_t last_sequence_;
    uint64_t log_number_;
    uint64_t prev_log_number_;  // 0 or backing store for memtable being compacted

    // Opened lazily
    WritableFile *descriptor_file_;
    log::Writer *descriptor_log_;

    // Per-level key at which the next compaction at that level should start.
    // Either an empty string, or a valid InternalKey.
    std::string compact_pointer_[config::kNumLevels];

    // No copying allowed
    VersionSet(const VersionSet &);

    void operator=(const VersionSet &);
};

} // namespace leveldb

#endif //CDPFS_LEVELDB_DB_VERSION_SET_H
