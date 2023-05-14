#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main(int argc, char **argv)
{
    MPI_Init(NULL, NULL); // initialize MPI
    int wsize;
    MPI_Comm_size(MPI_COMM_WORLD, &wsize);
    int wrank;
    MPI_Comm_rank(MPI_COMM_WORLD, &wrank);
    if (wrank == 0)
    {
        // read the array
        int n;
        scanf("%d", &n);
        int arr[n];
        for (int i = 0; i < n; i++)
        {
            arr[i] = rand() % 10 + 1;
        }
        printf("Random array generated is:\n [ ");
        for (int i = 0; i < n; i++)
        {
            printf("%d ", arr[i]);
        }
        printf("]\n");
        int elem_to_send = n / wsize;
        int tag = 0;
        for (int i = 1; i < wsize; i++)
        { // send the size
            if (i != wsize - 1)
            {
                elem_to_send = n / wsize;
                MPI_Send(&elem_to_send, 1, MPI_INT, i, i + wsize, MPI_COMM_WORLD);
                MPI_Send(&arr[i * (elem_to_send)], elem_to_send, MPI_INT, i, i + wsize + 1, MPI_COMM_WORLD);
                continue;
            }
            // elements would be changed
            elem_to_send = n / wsize + n % wsize;
            MPI_Send(&elem_to_send, 1, MPI_INT, i, i + wsize, MPI_COMM_WORLD);
            MPI_Send(&arr[(wsize - 1) * (n / wsize)], elem_to_send, MPI_INT, i, i + wsize + 1, MPI_COMM_WORLD);
            // send the array
        }
        int ans = 0;
        for (int i = 0; i < n / wsize; i++)
            ans += arr[i];
        // recv the data into the local var s_rec
        int s_rec;
        for (int i = 1; i < wsize; i++)
        {
            s_rec = 0;
            MPI_Recv(&s_rec, 1, MPI_INT, i, i + wsize + 2, MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);
            ans += s_rec;
        }
        printf("Total sum of array is %d\n", ans);
    }
    else
    {
        // receive the size of elements
        int size;
        MPI_Recv(&size, 1, MPI_INT, 0, wrank + wsize, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
        int arr[size];
        MPI_Recv(arr, size, MPI_INT, 0, wrank + wsize + 1, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
        int local = 0;
        for (int i = 0; i < size; i++)
            local = local + arr[i];
        printf("\nProcess %d sending sum %d back to main...\n", wrank, local);
        MPI_Send(&local, 1, MPI_INT, 0, wrank + wsize + 2, MPI_COMM_WORLD);
    }
    MPI_Finalize();
}