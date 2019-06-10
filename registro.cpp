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

using namespace std;

void registrar(char** comandos, int* length)
{   
    char *n = (char*)"evaluator";
    
    
    if(*length%2 != 0){
        cerr<<"comando inválido\n";
        exit(EXIT_FAILURE);
    }
    else if(*length>2 && strcmp(comandos[2],"-n")==0){
                n = comandos[3];
    }
    
    int numeromuestra = 0;
    
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
    int size_head =  sizeof(struct head);
    int size_exam =  sizeof(struct exam);
    dir= (void*) ((char *)dir+size_head);
    int entra=1;
    
    while (entra)
    {

        int bandeja, cantidad;
        char tipo;
        cout << "Ingresa un examen <integer> {B|S|D} <integer>" << endl;
        cin >> bandeja;
        cin >> tipo;
        cin >> cantidad;
        tipo = toupper(tipo);
        if (cin.fail()){
            entra=0;
            break;
        }
        
        while (!(0 <= bandeja && bandeja < i && tipo == 'B' || tipo == 'D' || tipo == 'S' && cantidad > 0)){
            cout << "Muestra incorrecta, vuelve a intentarlo" << endl;
            cout << "Ingresa un examen <integer> {B|S|D} <integer>" << endl;
            cin >> bandeja >> tipo >> cantidad;
            tipo = toupper(tipo);
            if (cin.fail()){
                entra=0;
                break;
            }
        }
        
        
        string mutex_n =("inp_"+ to_string(bandeja)+"_mutex").c_str();
        string full_n =("inp_"+ to_string(bandeja)+"_full").c_str();
        string empty_n =("inp_"+to_string(bandeja)+"_empty").c_str();
        
        sem_t *mutex = sem_open(mutex_n.c_str(),0);
        sem_t *full = sem_open(full_n.c_str(),0);
        sem_t *empty = sem_open(empty_n.c_str(),0);
        
        sem_wait(empty);
        sem_wait(mutex);
    
        int last;
        int s = sem_getvalue(full,&last);
        
        int tamanoentrada=size_exam *(bandeja)*ie;
        int tamanoregistros=size_exam*last;
        cout << last <<endl;


        dir= (void*) ((char *)dir+tamanoentrada+tamanoregistros);
        struct exam *pExam = (struct exam *) dir;
        
        
        pExam->bandeja=bandeja;
        pExam->cantidad=cantidad;
        pExam->tipo=tipo;
        pExam->id=numeromuestra;
        pExam->state=1;
        
        
        cout <<"Id exam: "<< pExam->id << endl;
        numeromuestra++;

        sem_post(mutex);
        sem_post(full);
        
    }
}
