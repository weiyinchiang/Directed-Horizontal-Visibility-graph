#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void getDHVg(int N, double *data,
        int *in, int *out) {
    int i, j;
    for (i=0; i<N; i++) {
        in[i] = 1;
        out[i] = 1;
    }

    double ijmax; // max number between i and j
    for (i=0; i<N; i++) {
        ijmax = data[i+1];
        if (ijmax > data[i])  // no need to scan further
            continue;
        for (j=i+2; j<N; j++) { // i+1 is connected with i, so no need to check
            if (data[j] > ijmax) { // i->j
                ++in[j];
                ++out[i];
                ijmax = data[j];
            }
            
            if (data[j] > data[i]) // then i can connect to almost j
                break;
        }
    }
}

void getU01(int N, double *data) {
    srand(1000);
    int i;
    for (i=0; i<N; i++)
        data[i] = rand()/(double)RAND_MAX;
}

void getLogisticMap(int N, double *data) {
    srand(1000);
    double x = rand()/(double)RAND_MAX;

    int i;
    for (i=0; i<10000; i++) x = 4.0*x*(1.0-x);
    for (i=0; i<N; i++) {
        x = 4.0*x*(1.0-x);
        data[i] = x;
    }
}

void hist(int N, int *in, int *out) {
    int i, inmax=in[0], outmax=out[0];
    for (i=1; i<N; i++) {
        if (inmax < in[i]) inmax = in[i];
        if (outmax < out[i]) outmax = out[i];
    }

    int *inhist = (int *)calloc(inmax+1, sizeof(int));
    int *outhist = (int *)calloc(outmax+1, sizeof(int));
    for (i=0; i<N; i++) {
        ++inhist[in[i]];
        ++outhist[out[i]];
    }
    
    for (i=1; i<=inmax; i++) 
        printf("%d ", inhist[i]);
    printf("\n");
    for (i=1; i<=outmax; i++) 
        printf("%d ", outhist[i]);
    printf("\n");

    // KL divergence
    int max = (inmax > outmax) ? inmax : outmax;
    double KLD = 0.0;
    for (i=0; i<=max; i++) 
        if (inhist[i] && outhist[i])
            KLD += (double)outhist[i] * log((double)outhist[i]/(double)inhist[i]);
    KLD /= N;
    printf("KLD = %.12f\n", KLD);

    free(inhist);
    free(outhist);
}

int main () {
    int N = 1000000;
    double *data = (double *)malloc(N*sizeof(double));
    int *in = (int *)malloc(N*sizeof(int));
    int *out = (int *)malloc(N*sizeof(int));

    //getU01(N, data);
    getLogisticMap(N, data);
    getDHVg(N, data, in, out);

    hist(N, in, out);

    free(data);
    free(in);
    free(out);

    return 0;
}
