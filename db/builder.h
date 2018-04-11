//
// Created by sjw on 11/04/2018.
//

#ifndef CDPFS_LEVELDB_DB_BUILDER_H
#define CDPFS_LEVELDB_DB_BUILDER_H

#include "leveldb/status.h"

namespace leveldb {


struct Options;
struct FileMetaData;

class Env;
class Iterator;
class TableCache;


Status BuildTable(const std::string& dbname,
                  Env* env,
                  const Options& options,
                  TableCache* table_cache,
                  Iterator* iter,
                  FileMetaData* meta);

}

} // namespace leveldb

#endif //CDPFS_LEVELDB_DB_BUILDER_H
