//
// Created by sjw on 01/04/2018.
//

#ifndef CDPFS_LEVELDB_INCLUDE_DUMPFILE_H
#define CDPFS_LEVELDB_INCLUDE_DUMPFILE_H

#include <string>
#include "leveldb/env.h"
#include "leveldb/export.h"
#include "leveldb/status.h"

namespace leveldb {

// Dump the contents of the file named by fname in text format to
// *dst.  Makes a sequence of dst->Append() calls; each call is passed
// the newline-terminated text corresponding to a single item found
// in the file.
//
// Returns a non-OK result if fname does not name a leveldb storage
// file, or if the file cannot be read.
LEVELDB_EXPORT Status DumpFile(Env* env, const std::string& fname,
                               WritableFile* dst);

}  // namespace leveldb

#endif //CDPFS_LEVELDB_INCLUDE_DUMPFILE_H
