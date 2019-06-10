#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <semaphore.h>
#include "registros.h"
#include "delmemory.h"

using namespace std;

void
del(const char * n) {

   int fd = shm_open(n, O_RDWR, 0660);
    if (fd < 0) {
        cerr << "Error abriendo la memoria compartida:en registro " << errno << strerror(errno) << endl;
        exit(1);
    }
    //obtener variables
    int i, ie;

    void *dir;
    if ((dir = mmap(NULL, sizeof(struct head), PROT_READ | PROT_WRITE, MAP_SHARED,
		fd, 0)) == MAP_FAILED) {
        cerr << "Error mapeando la memoria compartida: "
	    << errno << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }
    head *pHead = (head * )dir;
    i = pHead->i;
    ie = pHead->ie;



  for (int iteration=0; iteration<i; iteration++){

        string mutex_n =("inp_"+ to_string(iteration)+"_mutex").c_str();
        string full_n =("inp_"+ to_string(iteration)+"_full").c_str();
        string empty_n =("inp_"+to_string(iteration)+"_empty").c_str();

        

        sem_unlink(mutex_n.c_str());
        sem_unlink(full_n.c_str());
        sem_unlink(empty_n.c_str());


     

    }
    cout << "eliminando"<< endl;

  

}

