DIREXE := exec/

MCC := mpicc
CC := gcc
RUN := mpirun

all : dirs data toroidal solution

dirs:
	mkdir -p $(DIREXE)

data:
	$(CC) gen_data.c -o $(DIREXE)gen_data

toroidal:
	$(MCC) toroidal.c -o $(DIREXE)toroidal

solution:
	./$(DIREXE)gen_data 16
	$(RUN) -n 16 ./$(DIREXE)toroidal 

clean: 
	rm -rf *~ core $(DIROBJ) $(DIREXE) $(DIRHEA)*~ $(DIRSRC)*~ 
	rm datos.dat


