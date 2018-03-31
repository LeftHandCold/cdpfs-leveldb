cmake_minimum_required(VERSION 3.8)
project(cdpfs_leveldb)

include_directories(include)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp include/leveldb/db.h include/leveldb/export.h include/leveldb/options.h include/leveldb/status.h include/leveldb/slice.h include/leveldb/iterator.h include/leveldb/table.h include/leveldb/table_builder.h)
add_executable(cdpfs_leveldb ${SOURCE_FILES})