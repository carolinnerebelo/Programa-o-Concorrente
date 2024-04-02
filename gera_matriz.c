#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 100

void gera(long int l, long int c) {
    float elem = 0;
    //float ** resultado;

    printf("%ld %ld\n", l, c); // multiplica_matriz vai receber do arquivo primeiro a qtd de linhas e depois a qtd de colunas

    // imprime o primeiro elemento a00 da matriz A
    elem = (rand() % MAX) / 3.0;
    printf("%f ", elem);

    for (int i = 0; i < (l*c) - 1; i++) {
        elem = (rand() % MAX) / 3.0;
        printf("%f ", elem);
    }
   
    printf("\n");
}

int main(int argc, char * argv[]) {
    if (argc < 5) {
        printf("Insira a dimensao da matriz A e da matriz B: %s <linhas A> <colunas A> <linhas B> <colunas B>\n", argv[0]);
        exit(-1);
    }

    long int lA, cA, lB, cB;

    lA = atol(argv[1]);
    cA = atol(argv[2]);
    lB = atol(argv[3]);
    cB = atol(argv[4]);

    if (lA < 1 || cA < 1 || lB < 1 || cB < 1) {
        printf("A quantidade de linhas e colunas deve ser maior que zero\n");
        exit(-1);
    }

    if (cA != lB) {
        printf("Nao eh possivel fazer a multiplicacao de matrizes pois a quantidade de colunas de A deve ser igual a quantidade de linhas de B\n");
        exit(-1);
    }


    srand(time(NULL));
    
    gera(lA, cA);
    gera(lB, cB);

    return 0;
}

/*

    float ** matriz = (float**) malloc (l * sizeof(float*));;

    for (int i = 0; i < l; i++) {
        matriz[i] = (float*) malloc (c * sizeof(float));
    }

*/


/*    for (int i = 0; i < l-1; i++) {
        elem = (rand() % MAX) / 3.0;
        printf("%f\n", elem);
        for (int j = 0; j < c-1; j++) {
            elem = (rand() % MAX) / 3.0;
            printf("%f ", elem);
        }
    }*/