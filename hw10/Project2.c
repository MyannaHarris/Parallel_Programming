#include "sys/time.h"
#include "stdio.h"
#include "stdlib.h"
#include "pthread.h"

float *arrayA;
float *arrayB;
float *arrayC;
int n;
int nthreads;

struct threadParams{
        int start;
        int end;
};

void MatrixThread(void * param)
{
        struct threadParams* data = param;
        int i;
        int j;
        int k;
        for (i = data->start; i < data->end; i++) {
            for (j = 0; j < n; j++) {
                arrayC[i*n+j] = 0.0;
                for (k = 0; k < n; k++) {
                   arrayC[i*n+j] += arrayA[i*n+k]*arrayB[k*n+j];
                }
            }
        }
}

int main(int argc, char *argv[]) {
        struct timeval tpstart, tpend;
        float timeuse;

        if(argc != 3)
                fprintf(stderr, "Usage: Project2 n, nthreads\n"), exit(EXIT_FAILURE);
        int i;
        int j;

        n = atoi(argv[1]);
        nthreads = atoi(argv[2]);

        pthread_t threads[nthreads];

        arrayA = (float*)calloc(n*n,sizeof(double));
        arrayB = (float*)calloc(n*n,sizeof(double));
        arrayC = (float*)calloc(n*n,sizeof(double));

        for (i=0; i<n; i++)
                for (j=0;j<n;j++)
                {
                        arrayA[i*n+j]=2;
                        arrayB[i*n+j]=2;
                        arrayC[i*n+j]=0;
                }

        gettimeofday(&tpstart,NULL);
        int a = 0;
        int z = 0;

        for (i = 0; i < nthreads-1; i++)
        {
                struct threadParams*data = (struct threadParams*) malloc(sizeof(struct threadParams));
                z += n/nthreads;
                data->start = a;
                data->end = z;
                pthread_create(&threads[i], NULL, (void *)MatrixThread, (void *)data);
                a = z;
        }
        struct threadParams*data = (struct threadParams*) malloc(sizeof(struct threadParams));
        data->start = a;
        data->end = n;
        pthread_create(&threads[nthreads-1], NULL,(void *) MatrixThread, (void *)data);

        for (i = 0; i < nthreads; i++)
                pthread_join(threads[i], NULL);


        gettimeofday(&tpend,NULL);
        timeuse=1000000*(tpend.tv_sec-tpstart.tv_sec)+(tpend.tv_usec-tpstart.tv_usec);
        printf("ammount of time 1 = %f\n",timeuse);

        if (n<=40)
        {
                printf("\n[");
                for (i=0; i<n; i++)
                {
                        for (j=0; j<n; j++)
                                printf( "%f, ",arrayC[i*n+j]);
                        printf("\n  ");
                }
                printf("]");
        }
        free(arrayA);
        free(arrayB);
        free(arrayC);
        free(data);

}