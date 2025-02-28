/*
Programa para gerar os vetores de entrada
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 1000

int main(int argc, char * argv[]) {
    long int n;
    float elem = 0, resultado;

    if (argc < 2) {
        printf("Use %s <qtd de elementos>\n", argv[0]);
        exit(-1);
    }

    n = atol(argv[1]);

    if (n < 1) {
        printf("Erro: o numero de elementos deve ser maior que zero\n");
        exit(-1);
    }

    srand(time(NULL));

    printf("%ld\n", n);

    elem = (rand() % MAX) / 3.0;
    printf("%f ", elem);
    
    resultado = elem;

    for (int i = 0; i < n-1; i++) {
        elem = (rand() % MAX) / 3.0;
        printf("%f ", elem);

        resultado += elem;
    }

    printf("\n%f", resultado);

    return 0;
}