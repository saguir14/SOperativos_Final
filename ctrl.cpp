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
#include <time.h>
#include "include/ctrl.h"

using namespace std;
int i, ie;

int size_head = sizeof(struct head);
int size_exam = sizeof(struct exam);
//solo lista
void control(char** comandos, int* length)
{
    char *n = (char*)"evaluator";
    
    
    if(*length%2 != 0){
        cerr<<"comando inválido\n";
        exit(EXIT_FAILURE);
    }
    else if(*length>2 && strcmp(comandos[2],"-n")==0){
                n = comandos[3];
    }
    
   
        list(n);
    
}

void list( const char * n)
{
    int fd = shm_open(n, O_RDWR, 0660);

    if (fd < 0)
    {
        cerr << "Error abriendo la memoria compartida:en registro " << errno << strerror(errno) << endl;
        exit(1);
    }
    void *dir;
    if ((dir = mmap(NULL, size_head, PROT_READ | PROT_WRITE, MAP_SHARED,
                    fd, 0)) == MAP_FAILED)
    {
        cerr << "Error mapeando la memoria compartida: "
             << errno << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }
    head *pHead = (head *)dir;
    i = pHead->i;
    ie = pHead->ie;
    int size_in=ie*size_exam;
    
    //imprimir
   
    dir=(void *)(((char *)dir)+size_head);
    exam *pExam = (exam * )dir;
    
    for(int tt=0; tt<(i); tt++){

        for(int tt2=0; tt2<ie; tt2++){
            if(pExam->id!=0){
                
                cout <<pExam->id<<" ";
                cout <<pExam->bandeja<<" ";
                cout <<pExam->cantidad<<" ";
                cout <<pExam->state<<" ";
                cout <<pExam->tipo<<endl;
            }
     
            dir=(void *)(((char *)dir)+size_in*tt+tt2*size_exam);
            pExam = (exam * )dir;
        }
        cout <<endl;

    }
}
