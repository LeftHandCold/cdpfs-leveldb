//
// Created by sjw on 03/04/2018.
//

#include "port/port_posix.h"

#include <cstdlib>
#include <stdio.h>
#include <string.h>

namespace leveldb {
namespace port {

static void PthreadCall(const char* label, int result) {
    if (result != 0) {
        fprintf(stderr, "pthread %s: %s\n", label, strerror(result));
        abort();
    }
}

void InitOnce(OnceType* once, void (*initializer)()) {
    PthreadCall("once", pthread_once(once, initializer));
}


}
} // namespace leveldb
