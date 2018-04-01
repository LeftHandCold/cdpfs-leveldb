cmake_minimum_required(VERSION 3.8)
project(cdpfs_leveldb)

include_directories(include)
include_directories(.)
include(TestBigEndian)
test_big_endian(LEVELDB_IS_BIG_ENDIAN)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp include/leveldb/db.h include/leveldb/export.h include/leveldb/options.h include/leveldb/status.h include/leveldb/slice.h include/leveldb/iterator.h include/leveldb/table.h include/leveldb/table_builder.h include/leveldb/env.h include/leveldb/comparator.h include/leveldb/cache.h include/leveldb/write_batch.h include/leveldb/dumpfile.h include/leveldb/filter_policy.h db/db_impl.cc db/db_impl.h db/dbformat.h port/thread_annotations.h port/port.h port/port_posix.h port/atomic_pointer.h port/port_config.h db/log_writer.cc db/log_writer.h db/log_format.h util/options.cc util/env.cc util/env_posix.cc util/status.cc db/filename.h db/filename.cc)
add_executable(cdpfs_leveldb ${SOURCE_FILES})