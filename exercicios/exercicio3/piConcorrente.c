/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

// versão concorrente

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct{
    int nthreads;
    int id;
    long long int n;
} tArgs;

void * tarefa (void * arg) {
    tArgs * args = (tArgs*) arg;
    
    int nthreads = args->nthreads;
    long long int n = args->n;
    int id = args->id;
    
    double soma = 0, fator = 1;
    long long int i;
    
    for (i = id; i < n; i += nthreads) {
        soma = soma + fator / (2*i+1);
        fator = -fator;
    }
    printf("%f\n", soma);
    
    pthread_exit((void*) &soma);
}

double piSequencial (long long n) {
    double soma = 0.0, fator = 1.0;
    long long i;
    
    for (i = 0; i < n; i++) {
        soma = soma + fator/(2*i+1);
        fator = -fator;
    }
    
    return 4.0 * soma;
}

int main(int argc, char * argv[]) {
    if (argc < 3) {
        printf("Insira o valor de n e o numero de threads: %s <n> <nthreads>\n", argv[0]);
        exit(-1);
    }
    
    long long int n = atoll(argv[1]);
    int nthreads = atol(argv[2]);

    pthread_t tid[nthreads];
    
    double soma = 0, retorno, pi_estimado;
    
    for (int i = 0; i < nthreads; i++) {
        tArgs *args = (tArgs*) malloc(sizeof(tArgs));
        if (args == NULL) {
            printf("Erro ao alocar memoria\n");
            exit(-1);
        }
        
        args->id = i;
        args->nthreads = nthreads;
        args->n = n;
        
        if (pthread_create(&tid[i], NULL, tarefa, (void*) &args)) {
            printf("Erro\n");
            exit(-1);
        }
    }
    
    for (int i = 0; i < nthreads; i++) {
        if (pthread_join(tid[i], (void**) &retorno)) {
            printf("Erro\n");
            exit(-1);
        }
        soma += retorno;
    }
    
    pi_estimado = 4 * soma;
    
    printf("%f\n", pi_estimado);
    
    return 0;
}


