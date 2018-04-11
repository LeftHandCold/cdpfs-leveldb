//
// Created by sjw on 11/04/2018.
//
#include "db/builder.h"

#include "db/filename.h"
#include "db/dbformat.h"
#include "db/table_cache.h"
#include "leveldb/db.h"
#include "leveldb/env.h"
#include "leveldb/iterator.h"
#include "db/version_set.h"

namespace leveldb {


Status BuildTable(const std::string& dbname,
                  Env* env,
                  const Options& options,
                  TableCache* table_cache,
                  Iterator* iter,
                  FileMetaData* meta) {

    Status s;
    meta->file_size = 0;
    iter->SeekToFirst();

    std::string fname = TableFileName(dbname, meta->number);

    if (iter->Valid()) {
        WritableFile* file;
        s = env->NewWritableFile(fname, &file);

        if (!s.ok()) {
            s;
        }

        TableBuilder* builder = new TableBuilder(options, file);
    }

}

} // namespace leveldb
