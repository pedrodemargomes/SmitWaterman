FLAGS=-O3 -fPIC -Wall -DDEBUGMIN 

# CXX=icpc

RM=rm -f

EXEC= swParalelo
SERIAL= swSerial


all: $(EXEC)

$(EXEC):
	$(CXX) $(FLAGS) $(EXEC).c -c -o $(EXEC).o
	$(CXX) $(FLAGS) $(EXEC).o -o $(EXEC)

serial:
	$(CXX) $(FLAGS) $(SERIAL).c -c -o $(SERIAL).o
	$(CXX) $(FLAGS) $(SERIAL).o -o $(SERIAL)
clean:
	$(RM) $(EXEC).o $(EXEC) $(SERIAL)

run_serial:
	time ./(SERIAL) < entrada.txt >saidaSerial.txt

run_paralelo:
	time ./(EXEC) < entrada.txt >saidaParalela.txt