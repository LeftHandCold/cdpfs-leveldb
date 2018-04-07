//
// Created by sjw on 01/04/2018.
//

#include "leveldb/options.h"

#include "leveldb/comparator.h"
#include "leveldb/env.h"

namespace leveldb {

Options::Options()
        : comparator(BytewiseComparator()),
          create_if_missing(false),
          error_if_exists(false),
          paranoid_checks(false),
          env(Env::Default()),
          info_log(NULL),
          write_buffer_size(4<<20),
          max_open_files(1000),
          block_cache(NULL),
          block_size(4096),
          block_restart_interval(16),
          max_file_size(2<<20),
          compression(kSnappyCompression),
          reuse_logs(false),
          filter_policy(NULL) {
}

} // namespace leveldb
