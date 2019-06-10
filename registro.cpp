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

using namespace std;

void registrar(string n)
{

    int numeromuestra = 0;
    
    int fd = shm_open(n.c_str(), O_RDWR, 0660);
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
        
        int tamanoentrada=size_head *(bandeja-1)*ie;
        int tamanoregistros=size_exam *last;
        dir= (void*) ((char *)dir+tamanoentrada+tamanoregistros);
        struct exam *pExam = (struct exam *) dir;
        
        string muestraid = ("muestra_" + to_string(numeromuestra)).c_str();
        pExam->bandeja=bandeja;
        pExam->cantidad=cantidad;
        pExam->tipo=tipo;
        pExam->id=muestraid.c_str();
        
        cout <<"Id exam: "<< pExam->id << endl;
        numeromuestra++;

        sem_post(mutex);
        sem_post(full);
        
    }
}
int main(){
    registrar("evaluator");
    return 0;

}
