//
// Created by sjw on 10/04/2018.
//
#include "db/version_set.h"

#include <algorithm>
#include <stdio.h>
#include "db/filename.h"
#include "db/log_writer.h"
#include "db/memtable.h"
#include "db/table_cache.h"
#include "leveldb/env.h"
#include "leveldb/table_builder.h"
#include "util/coding.h"
#include "util/logging.h"

namespace leveldb {

static int TargetFileSize(const Options* options) {
    return options->max_file_size;
}

// Maximum bytes of overlaps in grandparent (i.e., level+2) before we
// stop building a single file in a level->level+1 compaction.
static int64_t MaxGrandParentOverlapBytes(const Options* options) {
    return 10 * TargetFileSize(options);
}

// Maximum number of bytes in all compacted files.  We avoid expanding
// the lower level file set of a compaction if it would make the
// total compaction cover more than this many bytes.
static int64_t ExpandedCompactionByteSizeLimit(const Options* options) {
    return 25 * TargetFileSize(options);
}

VersionSet::VersionSet(const std::string& dbname,
                       const Options* options,
                       TableCache* table_cache,
                       const InternalKeyComparator* cmp)
        : env_(options->env),
          dbname_(dbname),
          options_(options),
          table_cache_(table_cache),
          icmp_(*cmp),
          next_file_number_(2),
          manifest_file_number_(0),  // Filled by Recover()
          last_sequence_(0),
          log_number_(0),
          prev_log_number_(0),
          descriptor_file_(NULL),
          descriptor_log_(NULL) {
}

VersionSet::~VersionSet() {
    delete descriptor_log_;
    delete descriptor_file_;
}

bool VersionSet::ReuseManifest(const std::string& dscname,
                               const std::string& dscbase) {
    if (!options_->reuse_logs) {
        return false;
    }
    FileType manifest_type;
    uint64_t manifest_number;
    uint64_t manifest_size;
    if (!ParseFileName(dscbase, &manifest_number, &manifest_type) ||
        manifest_type != kDescriptorFile ||
        !env_->GetFileSize(dscname, &manifest_size).ok() ||
        // Make new compacted MANIFEST if old one is too big
        manifest_size >= TargetFileSize(options_)) {
        return false;
    }

    assert(descriptor_file_ == NULL);
    assert(descriptor_log_ == NULL);
    Status r = env_->NewAppendableFile(dscname, &descriptor_file_);
    if (!r.ok()) {
        Log(options_->info_log, "Reuse MANIFEST: %s\n", r.ToString().c_str());
        assert(descriptor_file_ == NULL);
        return false;
    }

    Log(options_->info_log, "Reusing MANIFEST %s\n", dscname.c_str());
    descriptor_log_ = new log::Writer(descriptor_file_, manifest_size);
    manifest_file_number_ = manifest_number;
    return true;
}

void VersionSet::MarkFileNumberUsed(uint64_t number) {
    if (next_file_number_ <= number) {
        next_file_number_ = number + 1;
    }
}




} // namespace leveldb