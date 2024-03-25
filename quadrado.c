#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int N;
int M;

int * vetor;

void * quadrado (void * arg) {
    int ident = * (int *) arg;

    for (int i = ident; i < N; i += M) {
        vetor[i] *= vetor[i];
    }

    pthread_exit(NULL);
}

void inicializa() {
    srand(time(NULL));

    vetor = (int*) malloc (N * sizeof(int));
    if (vetor == NULL) {
        printf("ERRO - Nao foi possivel alocar memoria\n");
        exit(-1);
    }

    for (int i = 0; i < N; i++) {
        vetor[i] = rand() % 10;
    }
}

int * copia(int *vtemp) {

    vtemp = (int*) malloc (N * sizeof(int));
    if (vtemp == NULL) {
        printf("ERRO - Nao foi possivel alocar memoria\n");
        exit(-1);
    }

    for (int i = 0; i < N; i++) {
        vtemp[i] = vetor[i];
    }
    return vtemp;
}

int verifica(int *vtemp) {
    int quadrado;
    for (int i = 0; i < N; i++) {
        quadrado = vtemp[i] * vtemp[i];
        if (quadrado != vetor[i]) {
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


int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("ERRO - Informe a quantidade de elementos do vetor e a quantidade de threads na linha de comando: <%s> <nelementos> <nthreads>", argv[0]);
        exit(-1);
    }

    N = atoi(argv[1]);
    M = atoi(argv[2]);

    pthread_t tid[M];
    int ident[M];

    int *vtemp;

    inicializa();
    vtemp = copia(vtemp);

    imprime();

    for (int i = 0; i < M; i++) {
        ident[i] = i;
        if (pthread_create(&tid[i], NULL, quadrado, (void *) &ident[i]))
            printf("ERRO\n");
    }

    for (int i = 0; i < M; i++) {
        if (pthread_join(tid[i], NULL))
            printf("ERRO\n");
    }

    imprime();

    if (verifica(vtemp)) {
        printf("Correto\n");
    } else {
        printf("Incorreto\n");
    }

    

    free(vetor);
    return 0;
}


