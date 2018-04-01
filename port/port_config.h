//
// Created by sjw on 01/04/2018.
//

#ifndef CDPFS_LEVELDB_PORT_PORT_CONFIG_H
#define CDPFS_LEVELDB_PORT_PORT_CONFIG_H

// Define to 1 if you have a definition for fdatasync() in <unistd.h>.
#if !defined(HAVE_FUNC_FDATASYNC)
#define HAVE_FUNC_FDATASYNC 0
#endif  // !defined(HAVE_FUNC_FDATASYNC)

// Define to 1 if you have Google CRC32C.
#if !defined(HAVE_CRC32C)
#define HAVE_CRC32C 0
#endif  // !defined(HAVE_CRC32C)

// Define to 1 if you have Google Snappy.
#if !defined(HAVE_SNAPPY)
#define HAVE_SNAPPY 0
#endif  // !defined(HAVE_SNAPPY)

// Define to 1 if your processor stores words with the most significant byte
// first (like Motorola and SPARC, unlike Intel and VAX).
#if !defined(LEVELDB_IS_BIG_ENDIAN)
#define LEVELDB_IS_BIG_ENDIAN 0
#endif  // !defined(LEVELDB_IS_BIG_ENDIAN)

// Define to 1 if you have the <atomic> header.
// NOTE: <atomic> was standardized in C++11, which will be required to build
// LevelDB soon.
#if !defined(LEVELDB_ATOMIC_PRESENT)
#define LEVELDB_ATOMIC_PRESENT 1
#endif  // !defined(LEVELDB_ATOMIC_PRESENT)

#endif //CDPFS_LEVELDB_PORT_PORT_CONFIG_H
