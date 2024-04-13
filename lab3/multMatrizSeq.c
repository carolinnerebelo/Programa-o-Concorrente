#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    long int l; // linha
    long int c; // coluna
} DimMatriz;

FILE * arq;

float ** aloca (float ** matriz, DimMatriz dim);
float ** inicializa (float ** matriz, DimMatriz dim);
void imprime (float ** matriz, DimMatriz dim);
float ** multiplica(float ** A, float ** B, float ** C, DimMatriz dimA, DimMatriz dimB);

int main(int argc, char * argv[]) {
    float ** A = NULL, ** B = NULL, ** C = NULL;
    DimMatriz dimA, dimB, dimC;

    if (argc < 4) {
        printf("Insira o nome dos arquivos de entrada e saida: %s <matrizA> <matrizB> <saida>\n", argv[0]);
        exit(-1);
    }

    // matriz A

    arq = fopen(argv[1], "rb");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo\n");
        exit(-1);
    }
    
    // le as dimensões da matriz A
    fread(&dimA.l, sizeof(dimA.l), 1, arq);
    fread(&dimA.c, sizeof(dimA.c), 1, arq);
    
    // inicializa matriz A
    A = aloca(A, dimA);
    A = inicializa(A, dimA);

    fclose(arq); // fecha o arquivo para abri-lo novamente com a matriz B

    // MATRIZ B

    arq = fopen(argv[2], "rb");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo\n");
        exit(-1);
    }

    // le as dimensões da matriz B
    fread(&dimB.l, sizeof(dimB.l), 1, arq);
    fread(&dimB.c, sizeof(dimB.c), 1, arq);

    // inicializa a matriz B
    B = aloca(B, dimB);
    B = inicializa(B, dimB);

    fclose(arq);

    // verifica se é possível multiplicar as matrizes de entrada
    if (dimA.c != dimB.l) {
        printf("Nao eh possivel fazer a multiplicacao de matrizes pois a quantidade de colunas de A deve ser igual a quantidade de linhas de B\n");
        exit(-1);
    }
    
    // aloca espaço para a matriz resultante
    dimC.l = dimA.l;
    dimC.c = dimB.c;
    C = aloca(C, dimC);

    C = multiplica(A, B, C, dimA, dimB);

    arq = fopen(argv[3], "wb");
    if (arq == NULL) {
        printf("Nao foi possivel abrir o arquivo\n");
        exit(-1);
    }

    // escreve os elementos da matriz resultante no arquivo
    for (long int i = 0; i < dimC.l; i++) {
        for (long int j = 0; j < dimC.c; j++) {
            fwrite(&(C[i][j]), sizeof(float), 1, arq);
        }
    }

    fclose(arq);

    free(A);
    free(B);
    free(C);

    return 0;
}

float ** aloca (float ** matriz, DimMatriz dim) {
    matriz = (float**) malloc (dim.l * sizeof(float*));
    if (matriz == NULL) {
        printf("Erro ao alocar memoria\n");
        exit(-1);
    }

    for (long int i = 0; i < dim.l; i++) {
        matriz[i] = (float*) malloc (dim.c * sizeof(float));
        if (matriz[i] == NULL) {
            printf("Erro ao alocar memoria\n");
            exit(-1);
        }
    }

    // inicializa com zeros pra não conter lixo
    for (long int i = 0; i < dim.l; i++) {
        for (long int j = 0; j < dim.c; j++) {
            matriz[i][j] = 0;
        }
    }

    return matriz;
}

float ** inicializa (float ** matriz, DimMatriz dim) {
    for (long int i = 0; i < dim.l; i++) {
        for (long int j = 0; j < dim.c; j++) {
            fread(&matriz[i][j], sizeof(matriz[i][j]), 1, arq);
        }
    }

    return matriz;
}

void imprime (float ** matriz, DimMatriz dim) {
    for (long int i = 0; i < dim.l; i++) {
        for (long int j = 0; j < dim.c; j++) {
            printf("%f ", matriz[i][j]);
        }
        printf("\n");
    }
}

float ** multiplica(float ** A, float ** B, float ** C, DimMatriz dimA, DimMatriz dimB) {
    // variáveis para calcular o tempo sequencial
    clock_t inicio, fim;
    double delta;

    inicio = clock();

    for (long int i = 0; i < dimA.l; i++) {
        for (long int j = 0; j < dimB.c; j++) {
            for (long int k = 0; k < dimB.l; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    fim = clock();
    delta = ((double) (fim - inicio)) / CLOCKS_PER_SEC;
    printf("Tempo sequencial: %f\n", delta);

    return C;
}