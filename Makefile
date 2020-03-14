DIREXE := exec/
MCC := mpicc
CC := gcc
RUN := mpirun
NODES := 16

all: dirs data toroidal new_data solution

dirs:
	mkdir -p $(DIREXE)

data:
	$(CC) gen_data.c -o $(DIREXE)gen_data

toroidal:
	$(MCC) toroidal.c -o $(DIREXE)toroidal

new_data:
	./$(DIREXE)gen_data $(NODES)

solution :
	$(RUN) -n $(NODES) ./$(DIREXE)toroidal 

clean: 
	rm -rf *~ core $(DIROBJ) $(DIREXE) $(DIRHEA)*~ $(DIRSRC)*~ 
	rm datos.dat


