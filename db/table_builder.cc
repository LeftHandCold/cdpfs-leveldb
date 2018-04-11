//
// Created by sjw on 11/04/2018.
//

#include "leveldb/table_builder.h"

#include <assert.h>

namespace leveldb {


TableBuilder::TableBuilder(const Options& options, WritableFile* file)
        : rep_(NULL) {
}

} // namespace leveldb
