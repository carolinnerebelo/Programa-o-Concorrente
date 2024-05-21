#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
Programa para gerar uma sequência de N inteiros e verificar quantos primos existem nessa sequência e armazená-los em um arquivo binário.
Os N primeiros elementos do arquivo binário são a própria sequência, e o último é a quantidade de primos.
A quantidade N de elementos da sequência e o nome desejado para o arquivo binário de saída devem ser passados na linha de comando. 
*/

// Função que verifica a primalidade de um número
int ehPrimo(long long int n) {
    int i;
    
    if (n<=1) return 0;
    if (n==2) return 1;
    if (n%2==0) return 0;
    
    for (i=3; i<sqrt(n)+1; i+=2)
        if(n%i==0) return 0;
    
    return 1;
}

int main(int argc, char * argv[]) {
    if (argc < 3) {
        printf("Insira o valor de N e o nome do arquivo de saida na linha de comando\n");
        exit(-1);
    }

    FILE *arq;
    long long int n = atoll(argv[1]);
    int qtd_primos = 0;

    arq = fopen(argv[2], "wb");

    if (arq == NULL) {
        printf("Erro - nao foi possivel abrir o arquivo\n");
        exit(-1);
    }

    for (int i = 1; i < n+1; i++) {
        fwrite(&i, sizeof(i), 1, arq);
        if (ehPrimo(i)) {
            qtd_primos++;
        }
    }

    fwrite(&qtd_primos, sizeof(qtd_primos), 1, arq);

    fclose(arq);

    return 0;
}