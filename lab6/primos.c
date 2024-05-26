#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

/*
Programa concorrente que implementa o padrão produtor/consumidor em um problema de primalidade.
O programa deve receber como argumentos na linha de comando: a quantidade de threads consumidoras, o tamanho do buffer e o nome do arquivo binário que contém a sequência a ser processada.
As threads consumidoras processam os elementos do buffer e verifica a primalidade de cada um. A quantidade de primos encontrada é armazenada em uma variável global, que no fim é comparada com a quantidade de primos correta encontrada no programa sequencial que gerou a sequência. A quantidade de primos só é impressa se a comparação for verdadeira. Imprime também a thread que mais encontrou primos.
*/

FILE *arq;

int M;                          // tamanho do buffer
long long int N;                // quantidade de elementos a serem processados

int i_global = 0;               // divisão dinâmica
int primos_global = 0;          // variável global para armazenar a quantidade total de primos
int vencedora;                  // thread vencedora

int *Buffer;

sem_t slotCheio, slotVazio;     // condicionais
sem_t mutexCons;                // exclusão mútua
sem_t mutex;                    // exclusão mútua

// #define TEMPO

// Função para verificar se um número é primo
int ehPrimo(long long int n) {
    int i;
    
    if (n<=1) return 0;
    if (n==2) return 1;
    if (n%2==0) return 0;
    
    for (i=3; i<sqrt(n)+1; i+=2)
        if(n%i==0) return 0;
    
    return 1;
}

// Função para calcular a quantidade de elementos presentes no arquivo. Retorna essa quantidade.
long long int tamanhoArquivo() {
    long long int n;
    long tam;
    
    fseek(arq, 0, SEEK_END);
    tam = ftell(arq);
    rewind(arq);

    n = tam / sizeof(int);
    
    return n;
}

// Função para inserir um elemento no buffer
void insere (int elemento) {
    static int in = 0;

    sem_wait(&slotVazio);
    Buffer[in] = elemento;
    in = (in + 1) % M;

    sem_post(&slotCheio);
}

// Função para retirar um elemento do buffer
int retira () {
    int elemento;
    static int out = 0;

    sem_wait(&slotCheio);
    sem_wait(&mutexCons);

    elemento = Buffer[out];
    out = (out + 1) % M;

    sem_post(&slotVazio);
    sem_post(&mutexCons);

    return elemento;
}

// Thread produtora
void *produtor (void *arg) {
    int elemento;

    for (int i = 0; i < N; i++) {
        if (fread(&elemento, sizeof(elemento), 1, arq)) {
            insere(elemento);   
        }
    }

    pthread_exit(NULL);
}

// Thread consumidora
void *consumidor (void *arg) {
    int id = (int) arg;
    int elemento, i_local, primos_local = 0;
    static int mais_primos = 0;

    // divisão dinâmica
    sem_wait(&mutex);
    i_local = i_global;
    i_global++;
    sem_post(&mutex);

    while (i_local < N) {
        elemento = retira();
    
        if (ehPrimo(elemento)) {
            primos_local++;
        }

        // divisão dinâmica
        sem_wait(&mutex);
        i_local = i_global;
        i_global++;
        sem_post(&mutex);
    }

    sem_wait(&mutex);
    primos_global += primos_local;
    sem_post(&mutex);

    printf("Thread %d encontrou %d primos\n", id, primos_local);

    // mais_primos é uma variável estática, inicializada com zero, que armazena a maior quantidade de primos encontrados por uma thread consumidora. se a quantidade que a thread atual encontrou for maior que a quantidade encontrada por uma thread anterior, a variável mais_primos é sobrescrita com a maior quantidade e a variável "vencedora" armazena o id da thread que está vencendo naquele momento
    sem_wait(&mutex);
    if (primos_local > mais_primos) {
        mais_primos = primos_local;
        vencedora = id;
    }
    sem_post(&mutex);

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {

    if (argc < 4) {
        printf("Insira na linha de comando: %s <quantidade de threads consumidoras> <tamanho do buffer> <nome do arquivo de entrada>\n", argv[0]);
        exit(-1);
    }

    int primos_correta;

    #ifdef TEMPO
    clock_t inicio, fim;
    double delta;
    #endif

    int qtd_cons = atoi(argv[1]);
    M = atoi(argv[2]);

    pthread_t tid[qtd_cons+1];

    // ABERTURA DO ARQUIVO BINÁRIO
    arq = fopen(argv[3], "rb");

    if (arq == NULL) {
        printf("Erro ao abrir o arquivo\n");
        exit(-1);
    }

    N = tamanhoArquivo() - 1;       // o arquivo tem N+1 elementos, pois tem também a quantidade de primos

    // INICIALIZAÇÃO DOS SEMAFOROS
    sem_init(&slotCheio, 0, 0);
    sem_init(&slotVazio, 0, M);
    sem_init(&mutexCons, 0, 1);
    sem_init(&mutex, 0, 1);

    // ALOCA MEMÓRIA PARA O BUFFER
    Buffer = (int*) calloc (M, sizeof(int));
    if (Buffer == NULL) {
        printf("Erro ao alocar memoria\n");
        exit(-1);
    }

    #ifdef TEMPO
    inicio = clock();
    #endif

    // CRIAÇÃO DA THREAD PRODUTORA
    if (pthread_create(&tid[0], NULL, produtor, NULL)) {
        printf("Erro\n");
        exit(-1);
    }

    // CRIAÇÃO DAS THREADS CONSUMIDORAS
    for (int i = 0; i < qtd_cons; i++) {
        if (pthread_create(&tid[i+1], NULL, consumidor, (void*) i+1)) {
            printf("Erro\n");
            exit(-1);
        }   
    }

    for (int i = 0; i < qtd_cons+1; i++) {
        if (pthread_join(tid[i], NULL)) {
            printf("Erro\n");
            exit(-1);
        }   
    }


    #ifdef TEMPO
    fim = clock();
    delta = ((double) (fim - inicio)) / CLOCKS_PER_SEC;
    #endif

    // LÊ O ÚLTIMO ELEMENTO DO ARQUIVO (quantidade de primos correta)
    if (!fread(&primos_correta, sizeof(primos_correta), 1, arq)) {
        printf("Nao foi possivel ler um elemento do arquivo\n");
    }

    // VERIFICA A CORRETUDE
    if (primos_correta != primos_global) {
        printf("O programa esta incorreto\n");
        exit(-1);
    }

    printf("Quantidade de primos: %d\n", primos_global);
    printf("Thread vencedora: %d\n", vencedora);

    #ifdef TEMPO
    printf("Tempo de execucao: %f\n", delta);
    #endif

    fclose(arq);
    free(Buffer);

    return 0;
}


