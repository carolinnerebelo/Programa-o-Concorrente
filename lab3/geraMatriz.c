#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 100

// FUNÇÃO AUXILIAR PARA GERAR UMA MATRIZ

// Devem ser passados como argumento: a quantidade de linhas, a quantidade de colunas e o nome desejado para o arquivo que vai armazenar a matriz

int main(int argc, char * argv[]) {
    FILE *arq;
    long int linhas, colunas;

    if (argc < 4) {
        printf("Insira a dimensao da matriz e o nome do arquivo a ser gerado: %s <linhas> <colunas> <nomearquivo>\n", argv[0]);
        exit(-1);
    }

    linhas = atol(argv[1]);
    colunas = atol(argv[2]);

    if (linhas < 1 || colunas < 1) {
        printf("A quantidade de linhas e colunas deve ser maior que zero\n");
        exit(-1);
    }

    arq = fopen(argv[3], "wb");

    if (arq == NULL) {
        printf("Erro - nao foi possivel abrir o arquivo\n");
        exit(-1);
    }
  
    srand(time(NULL));

    float elem = 0;
    long int qtd_elem = linhas * colunas;

    fwrite(&linhas, sizeof(linhas), 1, arq);
    fwrite(&colunas, sizeof(colunas), 1, arq);

    // gera aleatoriamente os elementos da matriz e escreve cada um no arquivo
    for (long int i = 0; i < qtd_elem; i++) {
        elem = (rand() % MAX) / 3.0;
        fwrite(&elem, sizeof(elem), 1, arq);
    }

    return 0;
}

