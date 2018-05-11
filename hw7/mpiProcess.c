/*
 * Myanna Harris
 * lab 4
 *
 * MPI - Process IDs
 */

#include <stdio.h>
#include <mpi.h>
#include <sys/types.h>
int main(int argc, char *argv[])
{
  int namelen;
  char processor_name[MPI_MAX_PROCESSOR_NAME];
  int rank, size;

  pid_t pid;
  int id = getpid();
  int idInit = id;
  MPI_Status status;
  pid_t* s_pid;
  pid_t* s_pidIn;
  s_pid = &idInit;
  s_pidIn = &idInit;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  int i;
  int minID = id;
  int minRank = rank;
  for (i=1; i<size; i++)
  {
    if (rank%2==0 || rank == 0)
    {
      MPI_Send(s_pid, 1, MPI_INT, (rank+1)%size, 1, MPI_COMM_WORLD);
      MPI_Recv(s_pidIn, 1, MPI_INT, (rank-1+size)%size, 1, MPI_COMM_WORLD, &status);
      s_pid = s_pidIn;
    }
    else
    {
      MPI_Recv(s_pidIn, 1, MPI_INT, (rank-1+size)%size, 1, MPI_COMM_WORLD, &status);
      MPI_Send(s_pid, 1, MPI_INT, (rank+1)%size, 1, MPI_COMM_WORLD);
      s_pid = s_pidIn;
    }

    if(minID > *s_pid)
    {
      minID = *s_pid;
      minRank = (rank-i+size)%size;
    }
  }
  printf("Process %d: my ID is %d;", rank, id);
  printf("the smallest process ID %d", minID);
  printf(" is the one from Process %d\n", minRank);
  MPI_Finalize();
  return 0;
}
