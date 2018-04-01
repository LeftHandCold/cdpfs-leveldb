
#include "leveldb/db.h"
#include <iostream>

int main() {
    leveldb::DB *db;
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, "testdb", &db);
    delete db;
	return 0;
}