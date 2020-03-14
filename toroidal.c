#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>


#define MAX_ITEMS   1024
#define L           4

#define FILE_NAME   "datos.dat"

#define TRUE        1
#define FALSE       0

#define NORTH       1
#define SOUTH       2
#define EAST        3
#define WEST        4

/* For data checks */
#define LENGTH_MSG "length"
#define SIZE_MSG   "size"


int end = FALSE;

void get_data(double *data, int *length){
    /* For load data from datos.dat */
    
    FILE *file;
    char *aux = malloc(MAX_ITEMS * sizeof(char));
    char *n;

    if ((file = fopen(FILE_NAME, "r")) == NULL){
        fprintf(stderr, "Error opening file.\n");
        end = TRUE; 
    }else{
        *length = 0;

        fgets(aux, MAX_ITEMS * sizeof(char), file);
        fclose(file);

        data[(*length)++] = atof(strtok(aux,","));

        while((n = strtok(NULL, ",")) != NULL)
            data[(*length)++] = atof(n);

    }

    free(aux);
    
}

void check_data(int var, char *type){
    /* For check length or size */

    if (var != L*L){
        fprintf(stderr, "Error in data %s\n", type);
        end = TRUE;
    }
}

void send_data(double *data){
    double buff_num;
    int i;

    for(i=0; i < L*L; i++){
        buff_num = data[i];
        MPI_Send(&buff_num, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
    }

    free(data);
}

int main(int argc, char *argv[]){
    double *data = malloc(MAX_ITEMS*sizeof(double));
    int length;
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    if (rank == 0){
        get_data(data, &length);
        check_data(length, LENGTH_MSG);
        check_data(size, SIZE_MSG);
        /* Broadcast confirmation */
        MPI_Bcast(&end,1,MPI_INT,0,MPI_COMM_WORLD);
        if (!end)
            send_data(data);
    }else{
        /* Get confirmation to continue from first node */
        MPI_Bcast(&end,1,MPI_INT,0,MPI_COMM_WORLD);
        
        if(!end){
            double num;

            /* Wait the number */
            MPI_Recv(&num, 1, MPI_DOUBLE, 0, MPI_ANY_TAG, MPI_COMM_WORLD, NULL);
            printf("%.2f\n",num);
        }
    }

    MPI_Finalize();

    return EXIT_SUCCESS;
}