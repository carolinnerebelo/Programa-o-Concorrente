#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

#define N 10
#define M 5

int vetor[N];

void * quadrado (void * arg) {
    int ident = * (int *) arg;

    for (int i = ident; i < N; i += M) {
        vetor[i] *= vetor[i];
    }

    pthread_exit(NULL);
}

void inicializa() {
    srand(time(NULL));

    for (int i = 0; i < N; i++) {
        vetor[i] = rand() % 100;
    }
}

void copia(int vtemp[]) {
    for (int i = 0; i < N; i++) {
        vtemp[i] = vetor[i];
    }
}

int verifica(int vtemp[]) {
    for (int i = 0; i < N; i++) {
        if (pow(vtemp[i], 2) != vetor[i]) {
            return 1;
        }
    }
    return 0;
}

void imprime() {
    for (int i = 0; i < N; i++) {
        printf("%d ", vetor[i]);
    }
    printf("\n");
}


int main(void) {

    pthread_t tid[M];
    int ident[M];

    int vtemp[N];

    inicializa();
    copia(vtemp);
    //imprime();

    for (int i = 0; i < M; i++) {
        ident[i] = i;
        if (pthread_create(&tid[i], NULL, quadrado, (void *) &ident[i]))
            printf("ERRO\n");
    }

    for (int i = 0; i < M; i++) {
        if (pthread_join(tid[i], NULL))
            printf("ERRO\n");
    }

    if (verifica(vtemp)) {
        printf("Correto\n");
    }

    //imprime();

    return 0;
}