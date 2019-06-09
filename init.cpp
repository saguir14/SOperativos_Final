#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <semaphore.h>
#include "registros.h"
#include <iostream>
#include <cstdlib>
#include <cstring>

using namespace std;

//#include "comando_init.h"
//Read all the comandos 
void comando_init(char* comandos[], int* length){
    int i=5, ie=6, oe=10, b=100, d=100, s=100, q=6;
    char *n = (char*)"evaluator";
    if (*length == 2){
    }
    else if(*length%2 != 0){
        cerr<<"comando inválido\n";
        exit(EXIT_FAILURE);
    }
    else{
        for(int it=2; it<*length; it+=2){
            if(strcmp(comandos[it],"-i") == 0){
                i = atoi(comandos[it+1]);
                if(i <= 0){
                    cerr<<"-i <integer>"<<endl;
                    exit(EXIT_FAILURE);
                }
            }
            else if(strcmp(comandos[it],"-ie")==0){
                ie = atoi(comandos[it+1]);
                if(ie <= 0){
                    cerr<<"-ie <integer>"<<endl;
                    exit(EXIT_FAILURE);
                }
            }
            else if(strcmp(comandos[it],"-oe")==0){
                oe = atoi(comandos[it+1]);
                if(oe <= 0){
                    cerr<<"-oe <integer>"<<endl;
                    exit(EXIT_FAILURE);
                }
            }
            else if(strcmp(comandos[it],"-n")==0){
                n = comandos[it+1];
            }
            else if(strcmp(comandos[it],"-b")==0){
                b = atoi(comandos[it+1]);
                if(b <= 0){
                    cerr<<"-b <integer>"<<endl;
                    exit(EXIT_FAILURE);
                }
            }
            else if(strcmp(comandos[it],"-d")==0){
                d = atoi(comandos[it+1]);
                if(d <= 0){
                    cerr<<"-d <integer>"<<endl;
                    exit(EXIT_FAILURE);
                }
            }
            else if(strcmp(comandos[it],"-s")==0){
                s = atoi(comandos[it+1]);
                if(s <= 0){
                    cerr<<"-s <integer>"<<endl;
                    exit(EXIT_FAILURE);
                }
            }
            else if(strcmp(comandos[it],"-q")==0){
                q = atoi(comandos[it+1]);
                if(q <= 0){
                    cerr<<"-q <integer>"<<endl;
                    exit(EXIT_FAILURE);
                }
            }
        }
    }
    // Crear la memoria compartida e inicializar los valores
    int fd = shm_open(n, O_RDWR | O_CREAT | O_EXCL, 0660);
    if (fd < 0) {
        cerr << "Error creando la memoria compartida: "<< endl;
        exit(EXIT_FAILURE);
    }

    int size_head =  sizeof(struct head);
    int size_exam =  sizeof(struct exam);

    cout<<size_head <<endl;
    if (ftruncate(fd, size_head + i*ie*size_exam) != 0) {
        cerr << "Error creando la memoria compartida: "
	    << errno << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }

    void *dir;
    if ((dir = mmap(NULL, size_head, PROT_READ | PROT_WRITE, MAP_SHARED,
		fd, 0)) == MAP_FAILED) {
        cerr << "Error mapeando la memoria compartida: "
	    << errno << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }

    head *pHead = (head * )dir;
    cout<<"Head"<<endl;
    pHead->i = i;
    pHead->ie = ie; 
    cout<<(pHead->i)<<endl;
    cout<<pHead<<endl;
    cout<<dir<<endl;

    for(int it=0; it<i; it++){
        for(int it2=0; it2<ie; it2++){
            if ((dir = mmap(0,size_exam, PROT_READ | PROT_WRITE, MAP_SHARED,
		        fd, 0)) == MAP_FAILED) {
                cerr << "Error mapeando la memoria compartida: "
	            << errno << strerror(errno) << endl;
                exit(EXIT_FAILURE);
            }
        }
    }

    for(int it=0; it<oe; it++){
        if ((dir = mmap(0,size_exam, PROT_READ | PROT_WRITE, MAP_SHARED,
		        fd, 0)) == MAP_FAILED) {
                cerr << "Error mapeando la memoria compartida: "
	            << errno << strerror(errno) << endl;
                exit(EXIT_FAILURE);
            }
    }
    exam *pExam = (exam *)dir;
    cout<<"Examen"<<endl;
    cout<<&(pExam->id)<<endl;
    cout<<pExam<<endl;
    close(fd);
}