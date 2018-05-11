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

/*
 *  * This function compares and splits a local and a remote list
 *   * @param nlocal: the number of elements in both the local and remote list
 *    * @param elmnts: (starting address of) the local list
 *     * @ param relmnts: (starting address of) the remote list
 *      * @ param wspace: (starting address of) a work space needed by
 *       *   this function. It has the same size as elmnts and relmnts
 *        * @ param keepsmall: it determines whether the current list keeps
 *         *   the smaller half or larger half after elmnts and 
 *          * relmnts are combined and sorted.
 *           */
void CompareSplit(int nlocal, int *elmnts, int *relmnts, int *wspace, int keepsmall)
{ 
  int i, j, k;
  for(i=0; i<nlocal; i++)
  {  
     wspace[i] = elmnts[i];
  }
  
  if (keepsmall)
  { 
    for(i=j=k=0; k<nlocal; k++)
    { 
      if(j==nlocal || (i < nlocal && wspace[i] < relmnts[j]))
        elmnts[k] = wspace[i++];
      else
        elmnts[k] = relmnts[j++];
    }
  }
  else
  {
    for(i=k=nlocal-1, j=nlocal-1; k>=0; k--)
    {
      if(j==-1 || (i >= 0 && wspace[i] >= relmnts[j]))
        elmnts[k] = wspace[i--];
      else
        elmnts[k] = relmnts[j--];
    }
  }
}

int main(int argc, char *argv[])
{
  int n = atoi(argv[1]);

  int namelen;
  char processor_name[MPI_MAX_PROCESSOR_NAME];
  int rank, size;

  int *array;
  int *subarray;
  int *remotearray;
  int *workarray;

  double t1, t2;
  int a,b,c,i,j,k,l,m;
  MPI_Status status;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  int npes = size;
  int nlocal = (n/npes);
  int oddrank;
  int evenrank;

  if(rank == 0)
  {
    array = (int*)malloc(n*sizeof(int));
    for(i=0; i<n; i++)
    {
      array[i] = rand() % 100;
    }
  }
  subarray = (int*)malloc(nlocal*sizeof(int));
  remotearray = (int*)malloc(nlocal*sizeof(int));
  workarray = (int*)malloc(nlocal*sizeof(int));


  t1 = MPI_Wtime();

  MPI_Scatter(array,nlocal,MPI_INT,subarray,nlocal,MPI_INT,0,MPI_COMM_WORLD);

  //Do sort
  //bubbleSort(subarray,nlocal);
  quicksort(subarray,0,nlocal-1,1);
  if(rank%2 == 0)
  {
    oddrank = (rank-1);
    evenrank = (rank+1);
  }
  else
  {
    oddrank = (rank+1);
    evenrank = (rank-1);
  }

  if (oddrank == -1 || oddrank == size)
  {
    oddrank = MPI_PROC_NULL;
  }
  if (evenrank == -1 || evenrank == size)
  {
    evenrank = MPI_PROC_NULL;
  }

  for(i = 0; i<npes; i++)
  {
    if(i%2 == 1)
    {
      MPI_Sendrecv(subarray,nlocal,MPI_INT,oddrank,1,remotearray,nlocal,MPI_INT,oddrank,1,MPI_COMM_WORLD,&status);
      if(rank != 0 && rank!= size-1)
      {
        CompareSplit(nlocal,subarray,remotearray,workarray,rank<status.MPI_SOURCE);
      }
    }
    else
    {
      MPI_Sendrecv(subarray,nlocal,MPI_INT,evenrank,1,remotearray,nlocal,MPI_INT,evenrank,1,MPI_COMM_WORLD,&status);
      CompareSplit(nlocal,subarray,remotearray,workarray,rank<status.MPI_SOURCE);
    }
  }

  MPI_Gather(subarray,nlocal,MPI_INT,array,nlocal,MPI_INT,0,MPI_COMM_WORLD);

  t2 = MPI_Wtime();

  if (rank == 0)
  {
    double t = t2-t1;
    printf("Time elapsed is %f\n", t);
    if (n<=40)
    {
      for(a=0; a<n; a++)
      {
        printf("%d ", array[a]);
        printf("\n");
      }
    }
    free(array);
  }

  free(subarray);
  free(workarray);
  free(remotearray);

  MPI_Finalize();
  return 0;
}
