//
// Created by sjw on 01/04/2018.
//
// Log format information shared by reader and writer.

#ifndef CDPFS_LEVELDB_DB_LOG_FORMAT_H
#define CDPFS_LEVELDB_DB_LOG_FORMAT_H

namespace leveldb {
namespace log {

enum RecordType {
    // Zero is reserved for preallocated files
            kZeroType = 0,

    kFullType = 1,

    // For fragments
            kFirstType = 2,
    kMiddleType = 3,
    kLastType = 4
};
static const int kMaxRecordType = kLastType;

static const int kBlockSize = 32768;

// Header is checksum (4 bytes), length (2 bytes), type (1 byte).
static const int kHeaderSize = 4 + 2 + 1;

}  // namespace log
}  // namespace leveldb

#endif //CDPFS_LEVELDB_DB_LOG_FORMAT_H
