/*
 * Myanna Harris
 * lab 4
 *
 * MPI - Process IDs
 */

#include <stdio.h>
#include <mpi.h>
#include <sys/types.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
  int n = atoi(argv[1]);

  int namelen;
  char processor_name[MPI_MAX_PROCESSOR_NAME];
  int rank, size;

  double *matB;
  double *matA;
  double *matC;

  double t1, t2;
  int a,b,c,i,j,k,l,m;
  MPI_Status status;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  int npes = size;
  int rows = (n/(npes-1));
  int left = n%(npes-1);

  if(rank == 0)
  {
    matB = (double*)calloc(n*n,sizeof(double));
    matA = (double*)calloc(n*n,sizeof(double));
    matC = (double*)calloc(n*n,sizeof(double));
  }
  else if(rank == (size-1))
  {
    matB = (double*)calloc(n*n,sizeof(double));
    matA = (double*)calloc((rows+left)*n,sizeof(double));
    matC = (double*)calloc((rows+left)*n,sizeof(double));
  }
  else
  {
    matB = (double*)calloc(n*n,sizeof(double));
    matA = (double*)calloc(rows*n,sizeof(double));
    matC = (double*)calloc(rows*n,sizeof(double));
  }

  //int i, j;

  t1 = MPI_Wtime();
  if (rank == 0)
  {
    //int a, c;
    for(a=0; a<n; a++)
    {
      for(c=0; c<n; c++)
      {
        matB[a*n+c] =1.0;
        matA[a*n+c] =1.0;
      }
    }
    /*for(a=0; a<n; a++)
    {
      for(c=0; c<n; c++)
      {
        printf("%d ", matB[a*n+c]);
      }
      printf("\n");
    }*/
    for(i=1; i<size; i++)
    {
      MPI_Send(matB, n*n, MPI_DOUBLE, i%size, 1, MPI_COMM_WORLD);
      if (i == (size-1))
      {
        MPI_Send(&matA[(i-1)*n*(rows)], (rows+left)*n, MPI_DOUBLE, i%size, 1, MPI_COMM_WORLD);
        MPI_Recv(&matC[(i-1)*n*(rows)], (rows+left)*n, MPI_DOUBLE, i%size, 1, MPI_COMM_WORLD, &status);
      }
      else
      {
        MPI_Send(&matA[(i-1)*n*rows], rows*n, MPI_DOUBLE, i%size, 1, MPI_COMM_WORLD);
        MPI_Recv(&matC[(i-1)*n*rows], rows*n, MPI_DOUBLE, i%size, 1, MPI_COMM_WORLD, &status);
      }
    }
  if (n<=16)
  {
    //int a, c;
    for(a=0; a<n; a++)
    {
      for(c=0; c<n; c++)
      {
        printf("%f ", matC[a*n+c]);
      }
      printf("\n");
    }
  }
  }
  else if(rank == (size-1))
  {
    MPI_Recv(matB, n*n, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &status);
    MPI_Recv(matA, (rows+left)*n, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &status);
    //int a, c;
   /*printf("process size -1\n");
    for(a=0; a<n; a++)
    {
      for(c=0; c<n; c++)
      {
        printf("%d ", matB[a*n+c]);
      }
      printf("\n");
    }
    for(a=0; a<(rows+left); a++)
    {
      for(c=0; c<n; c++)
      {
        printf("%d ", matA[a*n+c]);
      }
      printf("\n");
    }*/
    //int k, l, m;
    for (k=0; k<(rows+left); k++)
    {
      for (l=0; l<n; l++)
      {
        for (m=0; m<n; m++)
        {
          matC[k*n+l]+=matA[k*n+m]*matB[m*n+l];
        }
      }
    }
    MPI_Send(matC, (rows+left)*n, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
  }
  else
  {
    MPI_Recv(matB, n*n, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &status);
    MPI_Recv(matA, rows*n, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &status);
    //int k, l, m;
    for (k=0; k<(rows); k++)
    {
      for (l=0; l<n; l++)
      {
        for (m=0; m<n; m++)
        {
          matC[k*n+l]+=matA[k*n+m]*matB[m*n+l];
        }
      }
    }
    MPI_Send(matC, rows*n, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
  }
  t2 = MPI_Wtime();
  if (rank == 0)
  {
    double t = t2-t1;
    printf("Time elapsed is %f\n", t);
  }

  free(matB);
  free(matA);
  free(matC);

  MPI_Finalize();
  return 0;
}
