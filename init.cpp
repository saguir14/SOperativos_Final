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
#include "include/registros.h"
#include "include/init.h"
#include <string>
#include "include/delmemory.h" 
#include <typeinfo> 

using namespace std;


//Read all the comandos 
void comando_init(char** comandos, int* length){

    

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
    //reiniciar semaphores
    del(n);

    //semaphores
    vector<sem_t *> mutex;
    vector<sem_t *> full;
    vector<sem_t *> empty;
    
    for (int iteration=0; iteration<i; iteration++){

        string mutex_n =("inp_"+ to_string(iteration)+"_mutex").c_str();
        string full_n =("inp_"+ to_string(iteration)+"_full").c_str();
        string empty_n =("inp_"+to_string(iteration)+"_empty").c_str();

        

        sem_t *newmutex = sem_open(mutex_n.c_str(), O_CREAT | O_EXCL, 0660, 1);
        sem_t *newfull = sem_open(full_n.c_str(), O_CREAT | O_EXCL, 0660, 0);
        sem_t *newempty =sem_open(empty_n.c_str(), O_CREAT | O_EXCL, 0660, ie);


        mutex.push_back(newmutex);
        full.push_back(newfull); 
        empty.push_back(newempty); 

    }
    

    // Crear la memoria compartida e inicializar los valores
    int fd = shm_open(n, O_RDWR | O_CREAT | O_TRUNC, 0660);
    if (fd < 0) {
        cerr << "Error creando la memoria compartida: inicial"<< endl;
        exit(EXIT_FAILURE);
    }

    int size_head =  sizeof(struct head);
    int size_exam =  sizeof(struct exam);


    if (ftruncate(fd, size_head + i*ie*size_exam + oe*size_exam + q*3*size_exam) != 0) {
        cerr << "Error creando la memoria compartida: truncate"
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
    
    pHead->i = i;
    pHead->ie = ie;
    pHead->oe = oe;
    pHead->d=d;
    pHead->n = n;
    pHead->b = b;
    pHead->s = s;
    pHead->q = q; 
/*
    dir=(void *)(((char *)dir)+size_head);
   
    int size_in=ie*size_exam;
    int size_ou=oe*size_exam;
    int con=-1;
    exam *pExam = (exam * )dir;
   //inicializar
    for(int tt=0; tt<(i); tt++){

        for(int tt2=0; tt2<ie; tt2++){
            pExam = (exam * )dir;
            pExam->bandeja=-1;
            pExam->cantidad=-1;
            pExam->state=-1;
            pExam->tipo='N';
            pExam->id=con;
            con--;
            
            dir=(void *)(((char *)dir)+size_in*tt+tt2*size_exam);
        }

    }
    
    //imprimir
    if ((dir = mmap(NULL, size_head, PROT_READ | PROT_WRITE, MAP_SHARED,
		fd, 0)) == MAP_FAILED) {
        cerr << "Error mapeando la memoria compartida: "
	    << errno << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }
    dir=(void *)(((char *)dir)+size_head);
    
    for(int tt=0; tt<(i); tt++){

        for(int tt2=0; tt2<ie; tt2++){
        pExam = (exam * )dir;
        cout <<pExam->bandeja<<" ";
        cout <<pExam->cantidad<<" ";
        cout <<pExam->state<<" ";
        cout <<pExam->tipo<<" ";
        cout <<pExam->id<<endl;
        
        dir=(void *)(((char *)dir)+size_in*tt+tt2*size_exam);
        }
        cout <<endl;

    }
    
    */
    close(fd);
}


