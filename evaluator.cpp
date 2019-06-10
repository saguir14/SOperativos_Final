#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include "include/registros.h"
#include "include/init.h"
#include "include/registro.h"
#include "include/ctrl.h"
//#include "rep_command.h"
//#include "stop_command.h"

using namespace std;


void comando_evaluador(char** comandos, int* length){
   
    char* comando_actual = comandos[1];
    if (strcmp(comando_actual,"init") == 0){
        comando_init(comandos, length);
    }else if (strcmp(comando_actual,"reg") == 0){
        registrar(comandos, length);
     
    }else if (strcmp(comando_actual,"ctlr") == 0){
        control(comandos, length);
        
    /*
    }else if (strcmp(comando_actual,"rep") == 0){
        //cout<<"comando rep\n";
    }else if(strcmp(comando_actual, "stop") == 0){
        //cout<<"comando stop\n"; */
    }else{
        cerr<<"Comando " << comando_actual << " no encontrado\n";
        exit(EXIT_FAILURE);
    }
    
}
int
main(int argc, char* argv[]){
    comando_evaluador(argv,&argc);
    exit(EXIT_SUCCESS);
    return 0;
}

