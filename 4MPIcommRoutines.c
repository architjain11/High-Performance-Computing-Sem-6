#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    MPI_Init(NULL, NULL); // initialize MPI

    int wsize;
    MPI_Comm_size(MPI_COMM_WORLD, &wsize);
    int wrank;
    MPI_Comm_rank(MPI_COMM_WORLD, &wrank);
    char processor_name[MPI_MAX_PROCESSOR_NAME]; // get processor name
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);
    printf("Hello world from process %s, rank %d out of %d processes\n\n",
           processor_name, wrank, wsize);
    if (wrank == 0)
    {
        char *message = "Hello!";
        MPI_Send(message, 6, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
    }
    else
    {
        char message[6];
        MPI_Recv(message, 6, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Message received!\n");
        printf("Message is : %s\n", message);
    }
    MPI_Finalize();
    return 0;
}