#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>


#define L           4
#define N_NODES     L*L

#define FILE_NAME   "datos.dat"

#define TRUE        1
#define FALSE       0

/* For neighbors */
#define NORTH       1
#define SOUTH       2
#define EAST        3
#define WEST        4
#define N_NEIGHBORS 4

/* For data checks */
#define LENGTH_MSG "length"
#define SIZE_MSG   "size"

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

void get_data(double *data, int *length);
void check_data(int var, char *type);
void send_data(double *data);
void check_data(int var, char *type);
void send_data(double *data);
void toroidal_neighbors(int rank, int neighbors[]);
void calculate_min(int rank, double num);

int end = FALSE;

int main(int argc, char *argv[]){
    double *data = malloc(N_NODES * sizeof(double));
    int length;
    int rank, size;
    double num;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    if (rank == 0){
        get_data(data, &length);
        check_data(length, LENGTH_MSG);

        if (!end) check_data(size, SIZE_MSG);

        if (!end) send_data(data);
    }

    /* Get confirmation to continue from first node */
    MPI_Bcast(&end,1,MPI_INT,0,MPI_COMM_WORLD);
        
    if(!end){
        /* Wait the number */
        MPI_Recv(&num, 1, MPI_DOUBLE, 0, MPI_ANY_TAG, MPI_COMM_WORLD, NULL);
        printf("Rank %d => %.2f\n",rank,num);
        calculate_min(rank, num);
    }
    
    MPI_Finalize();

    return EXIT_SUCCESS;
}

void get_data(double *data, int *length){
    /* For load data from datos.dat */
    
    FILE *file;
    char *aux = malloc(1024 * sizeof(char));
    char *n;

    if ((file = fopen(FILE_NAME, "r")) == NULL){
        fprintf(stderr, "Error opening file\n");
        end = TRUE; 
    }else{
        *length = 0;

        fscanf(file, "%s", aux);
        
        fclose(file);

        data[(*length)++] = atof(strtok(aux,","));

        while((n = strtok(NULL, ",")) != NULL)
            data[(*length)++] = atof(n);

    }

    free(aux);
    
}

void check_data(int var, char *type){
    /* For check length or size */

    if (var != N_NODES){
        fprintf(stderr, "Error in data %s\n", type);
        end = TRUE;
    }
}

void send_data(double *data){
    /* Send numbers to all the nodes */

    double buff_num;
    int i;

    for(i=0; i < N_NODES; i++){
        buff_num = data[i];
        MPI_Send(&buff_num, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
    }

    free(data);
}

void toroidal_neighbors(int rank, int neighbors[]){
    /* Calculate the neighbors */

    int row = rank / L;
    int column = rank % L;

    switch (row){
        /* 
            1 2
            | |
            3 4
        */
        case 0: /* Lower row */
            neighbors[SOUTH] = rank + L;
            neighbors[NORTH] = (L-1) * L + rank;
            break;
        case L-1: /* Uppper row */
            neighbors[SOUTH] = rank % L;
            neighbors[NORTH] = rank - L; 
            break;
        default: /* Central row */
            neighbors[SOUTH] = rank + L;
            neighbors[NORTH] = rank - L; 
            break;
    }

    switch(column){
        /* 
            1 -- 2
            3 -- 4
        */
        case 0: /* Left column */
            neighbors[WEST] = (L * (row+1))-1;
            neighbors[EAST] = rank + 1;
            break;
        case L-1: /* Right column */
            neighbors[WEST] = rank -1;
            neighbors[EAST] = row * L;
            break;
        default: /* Central column */
            neighbors[WEST] = rank -1;
            neighbors[EAST] = rank + 1;  
            break;
    }
}

void calculate_min(int rank, double num){
    /* Calculate the minimum number of all the nodes */

    int neighbors[N_NEIGHBORS];
    double his_num;
    int i;

    toroidal_neighbors(rank, neighbors);

    for(i=1; i < L; i++){
        /* Rows */
        MPI_Send(&num, 1, MPI_DOUBLE, neighbors[SOUTH], 1, MPI_COMM_WORLD);
        MPI_Recv(&his_num,1,MPI_DOUBLE,neighbors[NORTH],1,MPI_COMM_WORLD,NULL);
        num = MIN(num, his_num);

        /* Columns */
        MPI_Send(&num, 1, MPI_DOUBLE, neighbors[EAST], 1, MPI_COMM_WORLD);
        MPI_Recv(&his_num,1,MPI_DOUBLE,neighbors[WEST],1,MPI_COMM_WORLD,NULL);
        num = MIN(num, his_num);

    }

    if(rank == 0) printf("\nThe minimum number is: %.2f\n",num);
    
}