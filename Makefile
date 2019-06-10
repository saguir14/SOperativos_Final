all: evaluator registro 

evaluator:  evaluator.o init.o delmemory.o
	g++ -g -o evaluator init.o delmemory.o evaluator.o  -lrt -lpthread

registro: registro.o 
	g++ -g -o registro registro.o -lrt -lpthread


evaluator.o: evaluator.cpp 
	g++ -g -c evaluator.cpp
	
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
