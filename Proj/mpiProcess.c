/*
 * Myanna Harris
 * Quinn Walters
 * Proj
 *
 * MPI - Proj
 */

#include <stdio.h>
#include <mpi.h>
#include <sys/types.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
  //size of matrix
  int n = atoi(argv[1]);

  //Variables for MPI info
  int namelen;
  char processor_name[MPI_MAX_PROCESSOR_NAME];
  int rank, size;

  // 1D Arrays/Matrices
  double *matB;
  double *matA;
  double *matC;
  double *buff1;
  double *buff2;
  
  //request and end pointer
  int *endFlags;

  //time keeping vars
  double t1, t2;
  //Looping vars
  int a,b,c,i,j,k,l,m,end, rowC, tag;
  
  //MPI info
  MPI_Request reqs[4];
  MPI_Status status;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  int npes = size;

  //allocate memory
  if(rank == 0)
  {
    matB = (double*)calloc(n*n,sizeof(double));
    matA = (double*)calloc(n*n,sizeof(double));
    matC = (double*)calloc(n*n,sizeof(double));
    buff1 = (double*)calloc(n,sizeof(double));
    endFlags = (int *)calloc(size-1, sizeof(int));
  }
  else
  {
    matB = (double*)calloc(n*n,sizeof(double));
    matA = (double*)calloc(n,sizeof(double));
    matC = (double*)calloc(n,sizeof(double));
    buff1 = (double*)calloc(n,sizeof(double));
    buff2 = (double*)calloc(n,sizeof(double));
  }

  //master fills arrays
  if (rank == 0)
  {
    for(a=0; a<n; a++)
    {
      for(c=0; c<n; c++)
      {
        matB[a*n+c] =1.0;
        matA[a*n+c] =1.0;
      }
    }
    
    for(a=0; a<size-1; a++)
    {
      endFlags[a] = 0;
    }
  }

  t1 = MPI_Wtime();
  
  //Broadcast matrixB
  MPI_Bcast(matB, n*n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  
  if (rank == 0)
  {
    end = 0;
    rowC = 0;
    
    for(c=1; c<npes; c++)
    {
      MPI_Send(&matA[rowC*n], n, MPI_DOUBLE, c, rowC, MPI_COMM_WORLD);
      rowC++;
    }
    //master communicates
    while(end < 2*npes)
    {
      MPI_Recv(buff1, n, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      if(end > npes)
      {
        end++;
      }
      
      if(rowC < n)
      {
        MPI_Isend(&matA[rowC*n], n, MPI_DOUBLE, status.MPI_SOURCE, rowC, MPI_COMM_WORLD, &reqs[0]);
      }
      
      if(status.MPI_TAG < n)
      {
        for(c=0; c<n; c++)
        {
          matC[status.MPI_TAG*n+c] = buff1[c];
        }
      }
      
      if(rowC < n)
      {
        MPI_Wait(&reqs[0], &status);
        rowC++;
      }
      
      if(rowC >= n && rowC < n+size-1 && !endFlags[status.MPI_SOURCE-1])
      {
        MPI_Isend(buff1, n, MPI_DOUBLE, status.MPI_SOURCE, n+1, MPI_COMM_WORLD, &reqs[1]);
        MPI_Wait(&reqs[1], &status);
        endFlags[status.MPI_SOURCE-1] = 1;
        end++;
      }
    }
    
    t2 = MPI_Wtime();
    
    //Prints matrices from master if size of matrices <= 16
 	if (n<=16)
 	{
    	for(a=0; a<n; a++)
    	{
      		for(c=0; c<n; c++)
      		{
        		printf("%f ", matA[a*n+c]);
      		}
      		printf("\n");
    	}
    	
    	printf("\n");
    	
    	for(a=0; a<n; a++)
    	{
      		for(c=0; c<n; c++)
      		{
        		printf("%f ", matB[a*n+c]);
      		}
      		printf("\n");
    	}
    	
    	printf("\n");
    	
    	for(a=0; a<n; a++)
    	{
      		for(c=0; c<n; c++)
      		{
        		printf("%f ", matC[a*n+c]);
      		}
      		printf("\n");
    	}
    	
    	printf("\n");
  	}
  }
  else
  {
    //Slaves
    
    i=0;
    
    MPI_Irecv(buff1, n, MPI_DOUBLE, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &reqs[0]);
    MPI_Isend(matC, n, MPI_DOUBLE, 0, rank-1, MPI_COMM_WORLD, &reqs[3]);
    
    //Loop until no more work
    while(1)
    {
      //Switch between 2 buffs for reading and calculating
      if(i==0)
      {
		MPI_Irecv(buff2, n, MPI_DOUBLE, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &reqs[1]);
		
		MPI_Wait(&reqs[0], &status);
		tag = status.MPI_TAG;
		if(status.MPI_TAG == n+1)
		{
		  MPI_Wait(&reqs[3], &status);
		  break;
		}
		matA = buff1;
		
		for(a=0; a<n; a++)
	    {
	      matC[a] = 0.0;
	    }
		
		for (k=0; k<(1); k++)
		{
		  for (l=0; l<n; l++)
		  {
		    for (m=0; m<n; m++)
		    {
		      matC[k*n+l]+=matA[k*n+m]*matB[m*n+l];
		    }
		  }
		}

	    MPI_Wait(&reqs[3], &status);
        MPI_Isend(matC, n, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &reqs[3]);
        i=1;
      }
      else
      {
        MPI_Irecv(buff1, n, MPI_DOUBLE, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &reqs[0]);
		
		MPI_Wait(&reqs[1], &status);
		tag = status.MPI_TAG;
		if(status.MPI_TAG == n+1)
		{
		  MPI_Wait(&reqs[3], &status);
		  break;
		}
		matA = buff1;
		
		for(a=0; a<n; a++)
	    {
	      matC[a] = 0.0;
	    }
		
		for (k=0; k<(1); k++)
		{
		  for (l=0; l<n; l++)
		  {
		    for (m=0; m<n; m++)
		    {
		      matC[k*n+l]+=matA[k*n+m]*matB[m*n+l];
		    }
		  }
		}

	    MPI_Wait(&reqs[3], &status);
        MPI_Isend(matC, n, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &reqs[3]);
        i=0;
      }
    }
  }

  if (rank == 0)
  {
    //Print time it took
    double t = t2-t1;
    printf("Time elapsed is %f\n", t);
    
    free(endFlags);
  }
  else
  {
    //free slave buffs
    free(buff2);
  }

  //free all other buffs
  free(buff1);
  free(matB);
  free(matA);
  free(matC);

  MPI_Finalize();
  return 0;
}
