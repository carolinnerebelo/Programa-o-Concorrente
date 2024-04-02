#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/*typedef struct {
    int id;
    long int tamBloco;
} tArgs;*/

float ** matriz;

void inicializa (long int l, long int c) {
    matriz = (float**) malloc (l * sizeof(float*));
    if (matriz == NULL) {
        printf("Erro ao alocar memoria\n");
        exit(-1);
    }

    for (long int i = 0; i < l; i++) {
        matriz[i] = (float*) malloc (c * sizeof(float));
        if (matriz[i] == NULL) {
            printf("Erro ao alocar memoria\n");
            exit(-1);
        }
    }

    for (long int i = 0; i < l; i++) {
        for (long int j = 0; j < c; j++) {
            scanf("%f", &matriz[i][j]);
        }
    }

}

void imprime (long int l, long int c) {
    for (long int i = 0; i < l; i++) {
        for (long int j = 0; j < c; j++) {
            printf("%f ", matriz[i][j]);
        }
        printf("\n");
    }
}

void * tarefa (void * arg) {
    int * ident = (int *) arg;

    for (long int k = 0; k < ident; k++) // tem que ver a condição de parada que vai ser recebida como argumento da thread, criar uma struct

    pthread_exit(NULL);
}

int main() {
    long int lA, cA, lB, cB;
    int nthreads, *ident;
    
    pthread_t *tid;
    
    // le as dimensões da matriz A
    scanf("%ld %ld", &lA, &cA);

    printf("matriz %ldx%ld\n", lA, cA);

    // inicializa matriz A
    inicializa(lA, cA);
    imprime(lA, cA);

    // le as dimensões da matriz B
    scanf("%ld %ld", &lB, &cB);

    printf("matriz %ldx%ld\n", lB, cB);

    // inicializa matriz B
    inicializa(lB, cB);
    imprime(lB, cB);

    // threads

    nthreads = lA * cB; // qtd de threads sendo a qtd de elementos da matriz resultante
    ident = (int*) malloc (nthreads * sizeof(int));

    tid = (pthread_t *) malloc(nthreads * sizeof(pthread_t));
    if (tid == NULL) {
        fprintf(stderr, "Erro ao alocar memoria\n");
        exit(-1);
    }

    for (long int i = 0; i < lA; i++) {
        for (long int j = 0; cB; j++) {
            if (pthread_create(&tid[i], NULL, tarefa, (void*) ident[i])) {
                printf("Erro - pthread_create\n");
                exit(-1);
            }
        }
    }
    
    return 0;
}

/*
qual a qtd de threads?
*/