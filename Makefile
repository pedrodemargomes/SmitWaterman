DEBUGFLAGS= -DDEBUGMIN
FLAGS=-O3 -Wall $(DEBUGFLAGS) -lm 
PFLAGS= -fopenmp $(FLAGS)
CXX=gcc

RM=rm -f


EXEC= swParalelo
SERIAL= swSerial
GEN=inputGen

all:  paralelo serial gen

paralelo:
	$(CXX) $(PFLAGS)  $(EXEC).c -c -o $(EXEC).o
	$(CXX) $(FLAGS) $(EXEC).o -o $(EXEC)

serial:
	$(CXX) $(FLAGS) $(SERIAL).c -c -o $(SERIAL).o
	$(CXX) $(FLAGS) $(SERIAL).o -o $(SERIAL)

clean:
	$(RM) *.o $(EXEC) $(SERIAL) entradaGen.txt saidaSerial.txt saidaParalela.txt

gen:
		$(CXX) $(FLAGS) $(GEN).c -o $(GEN)


run_serial:
	time ./$(SERIAL) < entradaGen.txt >saidaSerial.txt

run_paralelo:
	time ./$(EXEC) < entrada.txt >saidaParalela.txt

run_gen:
	./$(GEN)  >entradaGen.txt

run_teste: †
	./(SERIAL) < entradaGen.txt >saidaSerial.txt ;./(EXEC) < entradaGen.txt >saidaParalela.txt ; diff saidaParalela.txt saidaSerial.txt