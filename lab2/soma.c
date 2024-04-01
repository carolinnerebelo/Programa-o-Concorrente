/*
Programa para somar elementos de um vetor
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define TESTE

float *vetor;

typedef struct {
    int id;
    long int tamBloco;
} tArgs;

void * tarefa(void * arg) {
    tArgs *args = (tArgs *) arg;
    
    long int ini = args->id * args->tamBloco;
    long int fim = ini + args->tamBloco;

    float *resultado;
    resultado = (float*) malloc (sizeof(float));

    if (resultado == NULL) {
        printf("Erro ao alocar memoria\n");
        exit(-1);
    }

    *resultado = 0;

    for (long int i = ini; i < fim; i++) {
        *resultado += vetor[i];
    }

    pthread_exit((void*) resultado);
}

void inicializa(long int n) {
    vetor = (float*) malloc(n * sizeof(float));
    if (vetor == NULL) {
        fprintf(stderr, "Erro ao alocar memoria\n");
        exit(-1);
    }

    for (long int i = 0; i < n; i++) {
        scanf("%f", &vetor[i]);
    }
}

int main (int argc, char * argv[]) {
    float resultado; // armazena o resultado da soma
    long int n; // tamanho do vetor
    long int tamBloco; // tamanho do bloco de cada thread
    int nthreads; // número de threads que serão criadas

    pthread_t *tid;
    float *retorno;

    #ifdef TESTE
    float resultadoCorreto; // valor de referência para os testes
    #endif

    if (argc < 2) {
        fprintf(stderr, "Digite %s <numero de threads>\n", argv[0]);
        exit(-1);
    }

    nthreads = atoi(argv[1]);
    printf("numero de threads = %d\n", nthreads);

    // carregar o vetor de entrada
    scanf("%ld", &n);
    printf("numero de elementos do vetor = %ld\n", n);
    inicializa(n);

    #ifdef TESTE
    scanf("%f", &resultadoCorreto);
    #endif

    resultado = 0;

    tid = (pthread_t *) malloc(nthreads * sizeof(pthread_t));
    if (tid == NULL) {
        fprintf(stderr, "Erro ao alocar memoria\n");
        exit(-1);
    }

    tamBloco = n / nthreads;

    if (!tamBloco) {
        printf("\nA quantidade de threads eh maior que a quantidade de elementos, a execucao sera sequencial\n");
    }

    for (int i = 0; i < nthreads; i++) {
        tArgs *args = (tArgs*) malloc(sizeof(tArgs));
        if (args == NULL) {
            printf("Erro ao alocar memoria\n");
            exit(-1);
        }
        args->id = i;
        args->tamBloco = tamBloco;
        
        if (pthread_create(&tid[i], NULL, tarefa, (void*) args)) {
            fprintf(stderr, "Erro - pthread_create\n");
            exit(-1);
        }
    }

    if (n % nthreads) {
        puts("\nMain com tarefa");
        for (long int i = n - (n % nthreads); i < n; i++) {
            resultado += vetor[i];
        }
    }

    for (int i = 0; i < nthreads; i++) { 
        if (pthread_join(*(tid+i), (void **) &retorno)) {
            fprintf(stderr, "Erro - pthread join\n");
            exit(-1);
        }
        resultado += *retorno;
    }

    printf("\nResultado: %f\n", resultado);

    #ifdef TESTE
        printf("\nResultado correto: %f\n", resultadoCorreto);
    #endif

    free(vetor);
    free(tid);


    return 0;
}
