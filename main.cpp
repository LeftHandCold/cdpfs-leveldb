
#include "leveldb/db.h"
#include <iostream>

using namespace std;
using namespace leveldb;

int main() {
    leveldb::DB *db;
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, "testdb", &db);
    db->Put(WriteOptions(), "", "");
    delete db;
	return 0;
}