#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <mpi.h>


#define MAX_ITEMS   1024
#define FILE_NAME   "datos.dat"
#define TRUE        1
#define FALSE       0
#define L           4
#define NORTH       1
#define SOUTH       2
#define EAST        3
#define WEST        4


void get_data(double *data, int *end, int *length){
    
    FILE *file;
    char *aux = malloc(MAX_ITEMS * sizeof(char));
    char *n;

    if ((file = fopen(FILE_NAME, "r")) == NULL)
    {
        fprintf(stderr, "Error opening file.\n");
        *end = TRUE; 
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

int main(int argc, char *argv[]){
    double *data;
    double buff_num;
    int length, end = FALSE;

    get_data(data, &end, &length);

}