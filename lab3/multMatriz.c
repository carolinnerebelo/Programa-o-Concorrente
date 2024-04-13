#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

// struct para a dimensão das matrizes
typedef struct {
    long int l; // linha
    long int c; // coluna
} DimMatriz;

// struct para os argumentos da thread
typedef struct {
    int id;
    long int tamBloco;
    DimMatriz dim;
} tArgs;

// matrizes e ponteiro para arquivo como variáveis globais
float ** A, ** B, ** C;
FILE * arq;

#define TESTE

// função para alocar memória para as matrizes
// recebe como argumento uma matriz e a dimensão dela, e retorna a matriz com espaço alocado
float ** aloca (float ** matriz, DimMatriz dim);

// função para iniciar as matrizes
// recebe como argumento uma matriz e a dimensão dela, e retorna a matriz preenchida com os valores do arquivo
float ** inicializa (float ** matriz, DimMatriz dim);

// função para imprimir as matrizes
// recebe como argumento uma matriz e a dimensão dela
void imprime (float ** matriz, DimMatriz dim);


// função que executa as threads
void * tarefa (void * arg) {
    tArgs * args = (tArgs*) arg;
    
    long int ini = args->id * args->tamBloco;
    long int fim = ini + args->tamBloco;
    long int l = args->dim.l, c = args->dim.c;

    for (long int i = ini; i < fim; i++) {
        for (long int j = 0; j < c; j++) {
            for (long int k = 0; k < l; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }	
    }

    pthread_exit(NULL);
}

int main(int argc, char * argv[]) {
    DimMatriz dimA, dimB, dimC; // dimensão das matrizes
    int nthreads; // número de threads
    long int tamBloco; // tamanho do bloco que vai ser executado pelas threads
    pthread_t *tid; 
    clock_t inicio, fim; 
    double delta; // variáveis para medir o tempo
    
    // INICIALIZAÇÃO
    inicio = clock();

    // verificação dos parâmetros de entrada
    if (argc < 5) {
        printf("Insira o nome dos arquivos de entrada e saida e o numero de threads %s <nome matriz A> <nome matriz B> <nome saida> <numero de threads>\n", argv[0]);
        exit(-1);
    }

    // NUMERO DE THREADS

    nthreads = atoi(argv[4]);

    // MATRIZ A

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

    // determina o tamanho de cada bloco que vai ser processado pela thread

    tamBloco = dimA.l / nthreads; // utilizando a estratégia de dividir por blocos de linhas, dividimos a quantidade de linhas da matriz A pelo número de threads

    if (!tamBloco) {
        printf("\nA quantidade de threads eh maior que a quantidade de linhas, a execucao sera sequencial\n");
    }

    fim = clock();
    delta = ((double) (fim - inicio)) / CLOCKS_PER_SEC; // calcula o tempo de inicialização

    // FIM INICIALIZAÇÃO

    // PROCESSAMENTO

    inicio = clock();

    tid = (pthread_t *) malloc(nthreads * sizeof(pthread_t));
    if (tid == NULL) {
        fprintf(stderr, "Erro ao alocar memoria\n");
        exit(-1);
    }

    for (int i = 0; i < nthreads; i++) {
        tArgs *args = (tArgs*) malloc(sizeof(tArgs)); // cria os argumentos para a thread
        if (args == NULL) {
            printf("Erro ao alocar memoria\n");
            exit(-1);
        }
        args->id = i;
        args->tamBloco = tamBloco;
        args->dim.l = dimB.l; // Para a terceira iteração, precisamos da quantidade de linhas de A ou a quantidade de colunas de B (tanto faz pois cA = lB necessariamente)
        args->dim.c = dimB.c; // A quantidade de colunas de B é usada na segunda iteração

	    if (pthread_create(&tid[i], NULL, tarefa, (void*) args)) {
            fprintf(stderr, "Erro - pthread_create\n");
            exit(-1);
        }
    }

    // executa sequencialmente se a divisão não for exata e sobrar elementos
    if (dimA.l % nthreads) {
        for (long int i = dimA.l - (dimA.l % nthreads); i < dimA.l; i++) {
            for (long int j = 0; j < dimB.c; j++) {
                 for (long int k = 0; k < dimB.l; k++) { 
			        C[i][j] += A[i][k] * B[k][j];
		        }
	        }	
        }
    }

    for (int i = 0; i < nthreads; i++) { 
        if (pthread_join(tid[i], NULL)) {
            fprintf(stderr, "Erro - pthread join\n");
            exit(-1);
        }
    }

    fim = clock();
    delta = ((double) (fim - inicio)) / CLOCKS_PER_SEC; // calcula o tempo de processamento

    // FIM PROCESSAMENTO

    // FINALIZAÇÃO

    inicio = clock();
    
    // cria o arquivo onde vai ser escrita a matriz resultante
    arq = fopen(argv[3], "wb");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo\n");
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
    free(tid);

    fim = clock();
    delta = ((double) (fim - inicio)) / CLOCKS_PER_SEC; // calcula o tempo de finalização
    
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
