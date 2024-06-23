#include "rsa.h"

FILE *arq;

int main(int argc, char *argv[]) {

    // Verifica se foram inseridas todas as informações na linha de comando
    if (argc < 4) {
        printf("Insira o nome do arquivo de entrada, o nome do arquivo de saida e o numero de threads");
        exit(-1);
    }

    srand(time(NULL));  // Inicializa o gerador de números aleatórios
    setlocale(LC_ALL, "");    // suportar UTF-8

    // Abertura do arquivo de entrada
    arq = fopen(argv[1], "r");
    if (arq == NULL) {
        printf("Erro ao abrir arquivo");
        exit(-1);
    }

    // Obtém o tamanho do arquivo
    fseek(arq, 0, SEEK_END);
    long tamanhoArq = ftell(arq);
    rewind(arq);

    char *mensagem;
    mensagem = (char*) calloc(tamanhoArq + 1, sizeof(char));    // +1 para o caractere nulo
    if (mensagem == NULL) {
        printf("Erro ao alocar memoria\n");
        exit(-1);
    }

    // Lê o arquivo
    fread(mensagem, sizeof(char), tamanhoArq, arq);
    mensagem[tamanhoArq] = '\0'; // Adiciona o terminador nulo

    #ifdef TESTE
    printf("Mensagem original:\n%s\n", mensagem);
    #endif

    fclose(arq);

    long long n, e, d;
    // Gera as chaves RSA
    gera_chave_rsa(&n, &e, &d);

    #ifdef TESTE
    // Exibe as chaves pública e privada
    printf("Chave pública: (n = %lld, e = %lld)\n", n, e);
    printf("Chave privada: (n = %lld, d = %lld)\n", n, d);
    #endif

    long long *numeros;     // Array para armazenar os números correspondentes aos caracteres
    numeros = (long long*) calloc(tamanhoArq, sizeof(long long));
    if (numeros == NULL) {
        printf("Erro ao alocar memoria\n");
        exit(-1);
    }

    // Converte a mensagem para números usando a tabela ASCII
    string_to_numbers(mensagem, numeros);

    int num_threads = atoi(argv[3]);

    // Tamanho de cada bloco de elementos que cada thread irá processar
    int elementos_por_thread = (tamanhoArq + num_threads - 1) / num_threads;

    // Aloca espaço para armazenar os IDs e threads
    pthread_t threads[num_threads];
    ThreadArgs args[num_threads];

    double start, finish, elapsed_c, elapsed_d;

    GET_TIME(start);
    // Criptografa a mensagem em blocos usando threads
    for (int i = 0; i < num_threads; ++i) {
        args[i].args = numeros;
        args[i].chave = e;
        args[i].n = n;
        args[i].inicio = i * elementos_por_thread;
        args[i].fim = (i + 1) * elementos_por_thread;
        if (args[i].fim > tamanhoArq) args[i].fim = tamanhoArq; //Garante que a última thread não ultrapasse o final do vetor
        if(pthread_create(&threads[i], NULL, criptografa, &args[i])){
            puts("ERRO-pthread_create");
        };
    }

    // Espera todas as threads terminarem
    for (int i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], NULL);
    }
    GET_TIME(finish);

    // Tempo gasto para criptografar
    elapsed_c = finish - start;
    printf("Tempo de criptografia com %d threads: %f segundos\n", num_threads, elapsed_c);

    #ifdef TESTE
    // Exibe a mensagem criptografada
    printf("Mensagem criptografada: ");
    for (int i = 0; i < tamanhoArq; ++i) {
        printf("%lld ", numeros[i]);
    }
    printf("\n");
    #endif

    GET_TIME(start);
    // Descriptografa a mensagem em blocos usando threads
    for (int i = 0; i < num_threads; ++i) {
        args[i].args = numeros;
        args[i].chave = d;
        args[i].n = n;
        args[i].inicio = i * elementos_por_thread;
        args[i].fim = (i + 1) * elementos_por_thread;
        if (args[i].fim > tamanhoArq) args[i].fim = tamanhoArq; //Garante que a última thread não ultrapasse o final do vetor
        if(pthread_create(&threads[i], NULL, descriptografa, &args[i])){
            puts("ERRO--pthread_create");
            return 5;
        }
    }

    // Espera todas as threads terminarem
    for (int i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], NULL);
    }

    GET_TIME(finish);

    // Tempo gasto para descriptografar
    elapsed_d = finish - start;
    printf("Tempo de descriptografia com %d threads: %f segundos\n", num_threads, elapsed_d);

    // Tempo total de execução
    printf("Tempo total de execucao: %f segundos\n", elapsed_c + elapsed_d);

    #ifdef TESTE
    puts("");
    // Exibe a mensagem descriptografada
    printf("ASCII- mensagem descriptografada: ");
    for (int i = 0; i < tamanhoArq; ++i) {
        printf("%lld ", numeros[i]);
    }
    puts("");
    #endif

    // Converte números de volta para caracteres
    numbers_to_string(numeros, tamanhoArq, mensagem);

    #ifdef TESTE
    // Exibe a mensagem descriptografada
    printf("Mensagem descriptografada original: %s\n", mensagem);
    #endif

    // Arquivo de saída
    arq = fopen(argv[2], "w");
    if (arq == NULL) {
        printf("Erro ao abrir arquivo");
        exit(-1);
    }

    fwrite(mensagem, sizeof(char), tamanhoArq, arq);
    fclose(arq);

    free(numeros);
    free(mensagem);

    return 0;
}