all: evaluator 

evaluator:  evaluator.o init.o delmemory.o registro.o ctrl.o 
	g++ -g -o evaluator init.o ctrl.o delmemory.o evaluator.o registro.o -lrt -lpthread

evaluator.o: evaluator.cpp 
	g++ -g -c evaluator.cpp
	
ctrl.o: ctrl.cpp 
	g++ -g -c ctrl.cpp

init.o: init.cpp 
	g++ -g -c init.cpp

registro.o: registro.cpp
	g++ -g -c registro.cpp

delmemory.o: delmemory.cpp
	g++ -g -c delmemory.cpp

clean:
	rm -f *.o
	rm -f *.*~ *~
	rm -f init evaluator
