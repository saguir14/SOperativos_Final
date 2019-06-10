#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <semaphore.h>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <vector>
#include "registros.h"
#include "init.h"
#include <string>

int* getdir(void *dir,int i,int ie){
    if ((dir = mmap(NULL, sizeof(struct exam), PROT_READ | PROT_WRITE, MAP_SHARED,
            fd, sizeof(i*ie))) == MAP_FAILED) {
            cerr << "Error mapeando : en registro " << errno << strerror(errno) << endl;
            exit(1);
        }   

}