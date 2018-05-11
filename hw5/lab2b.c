/*Myanna Harris
 * lab2b
 */

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

float ** A;
float ** B;
float ** C;
int n;
int NUM_THREADS;
int rows;
int leftOvers;

void *math(void *threadid)
{/*
  int i = 0;
  int j = 0;
  int k = 0;
  int start = (*((int*)threadid))*rows;
  int end = start+(rows-1);
  if ((*((int*)threadid)) == NUM_THREADS-1)
  {
    for(i=start; i<(end+leftOvers); i++)
    {
      for(j=start; i<(end+leftOvers); j++)
      {
        for(k=start; k<(end+leftOvers); k++)
        {
          C[i][j] += A[i][k]*B[k][j];
        }
      }
    }
  }
  else
  {
    for(i=start; i<end; i++)
    {
      for(j=start; i<end; j++)
      {
        for(k=start; k<end; k++)
        {
          C[i][j] += A[i][k]*B[k][j];
        }
      }
    }
  }*/
  pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
  n = atoi(argv[1]);
  NUM_THREADS = atoi(argv[2]);

  rows = n/NUM_THREADS;
  leftOvers = n%NUM_THREADS;

  pthread_t threads[NUM_THREADS];
  int rc, t;

  int i = 0;
  int j = 0;
  struct timeval tpstart, tpend;
  float timeuse;
  A = (float **)calloc(n, sizeof(float *));
  B = (float **)calloc(n, sizeof(float *));
  C = (float **)calloc(n, sizeof(float *));
  for (i = 0; i < n; i++)
  {
    A[i] = (float *)calloc(n, sizeof(float));
    B[i] = (float *)calloc(n, sizeof(float));
    C[i] = (float *)calloc(n, sizeof(float));
  }
  //initialization
  for(i=0; i<n; i++)
  {
    for(j=0; j<n; j++)
    {
      A[i][j] = 1.0;
      B[i][j] = 1.0;
      C[i][j] = 0.0;
    }
  }

  gettimeofday(&tpstart, NULL);

  for(t=0; t<NUM_THREADS; t++){
    rc = pthread_create(&threads[t], NULL, math, (void *)t);
    if (rc){
      printf("ERROR: return error from pthread_create() is %d\n", rc);
      exit(-1);
    }
  }

  for(t=0; t<NUM_THREADS; t++){
    rc = pthread_join(threads[t], NULL);
    if (rc){
      printf("ERROR: return error from pthread_join() is %d\n", rc);
      exit(-1);
    }
  }

  gettimeofday(&tpend, NULL);
  timeuse = 1000000*(tpend.tv_sec-tpstart.tv_sec)+(tpend.tv_usec-tpstart.tv_usec);

  if (n <= 40){  
    for(i = 0; i<n; i++)
    {
      for(j=0; j<n; j++)
      {
        printf("%f ", C[j]);
      }
      printf("\n");
    }
  }

  for(i = 0; i <n; i++)
  {
    free(A[i]);
    free(B[i]);
    free(C[i]);
  }
  free(A);
  free(B);
  free(C);

  printf("%f\n", timeuse);
  return 0;
}
