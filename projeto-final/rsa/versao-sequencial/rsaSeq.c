#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <locale.h>
#include "timer.h"

FILE *arq;

#define TESTE

// Função para calcular o Máximo Divisor Comum (MDC) de dois números
long long gcd(long long a, long long b) {
    while (b != 0) {
        long long t = b;
        b = a % b;
        a = t;
    }
    return a;
}

// Função para calcular o inverso modular usando a busca linear (e * d mod φ(n) = 1)
long long achad(long long e, long long phi) {
    long long i = 1;
    while (1) { // loop infinito até encontrar a solução
        if ((e * i) % phi == 1) {
            return i; // retorna o valor de i quando a condição é satisfeita
        }
        i++;
    }
}

// Função para calcular a exponenciação modular (base^exp % mod)
long long mod_exp(long long base, long long exp, long long mod) {
    // Inicializado como 1, pois qualquer número elevado a 0 é 1
    long long result = 1;

    // Quando o número não é elevado a 0
    while (exp > 0) {
        // Se exp é ímpar, multiplica base com o resultado
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
       
        // exp é agora par
        exp = exp / 2;
        base = (base * base) % mod;
    }
    return result;
}

// Função para verificar se um número é primo
int ehPrimo(long long int n) {
    int i;
    if (n <= 1) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    for (i = 3; i <= sqrt(n); i += 2)
        if (n % i == 0) return 0;
    return 1;
}

// Função para gerar um número primo
long long gera_primo() {
    long long num;
    int primoEncontrado = 0;

    while (!primoEncontrado) {
        // Gera um número aleatório entre 10000 e 19999
        num = rand() % 10000 + 10000;

        if (ehPrimo(num)) {
            primoEncontrado = 1;
        }
    }
    return num;
}

// Função para gerar as chaves RSA
void gera_chave_rsa(long long *n, long long *e, long long *d) {
    long long p = gera_primo();
    long long q = gera_primo();

    // Garante que q seja diferente de p (gera q até que seja diferente de p)
    while (q == p) {
        q = gera_primo();
    }

    // Calcula n = p * q
    *n = p * q;

    // Calcula φ(n) = (p - 1) * (q - 1)
    long long phi = (p - 1) * (q - 1);

    // Escolhe e tal que 1 < e < φ(n) e gcd(e, φ(n)) = 1
    *e = 3;

    // Vai somando 1 até gcd(e, φ(n)) = 1
    while (gcd(*e, phi) != 1) {
        (*e)++;
    }

    // Calcula d tal que (d * e) % φ(n) = 1
    *d = achad(*e, phi);

    #ifdef TESTE
    // Imprime primos e o totiente
    printf("Primo gerado p: %lld\n", p);
    printf("Primo gerado q: %lld\n", q);
    printf("Valor de φ(n): %lld\n", phi);
    #endif
}

// Função para converter uma string em números usando a tabela ASCII
void string_to_numbers(char *str, long long *numeros) {
    int len = strlen(str);
    for (int i = 0; i < len; i++) {
        numeros[i] = (long long) str[i];
    }
}

// Função para converter números de volta para caracteres
void numbers_to_string(long long *numeros, int len, char *str) {
    for (int i = 0; i < len; i++) {
        str[i] = (char) numeros[i];
    }
    str[len] = '\0'; // Adiciona o terminador nulo
}


int main(int argc, char *argv[]) {

    // Verifica se foram inseridas todas as informações na linha de comando
    if (argc < 3) {
        printf("Insira o nome do arquivo de entrada e o nome do arquivo de saida");
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
    // Imprime a mensagem original lida do arquivo
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

    double start, finish, elapsed_c, elapsed_d;

    GET_TIME(start);
    // Criptografa a mensagem
    for (int i = 0; i < tamanhoArq; i++) {
        numeros[i] = mod_exp(numeros[i], e, n);
    }
    GET_TIME(finish);

    elapsed_c = finish - start;

    // Tempo gasto para criptografar
    elapsed_c = finish - start;
    printf("Tempo de criptografia: %f segundos\n", elapsed_c);

    #ifdef TESTE
    // Exibe a mensagem criptografada
    printf("Mensagem criptografada: ");
    for (int i = 0; i < tamanhoArq; ++i) {
        printf("%lld ", numeros[i]);
    }
    printf("\n");
    #endif

    GET_TIME(start);
    // Descriptografa a mensagem
    for (int i = 0; i < tamanhoArq; ++i) {
        numeros[i] = mod_exp(numeros[i], d, n);
    }
    GET_TIME(finish);

    // Tempo gasto para descriptografar
    elapsed_d = finish - start;
    printf("Tempo de descriptografia: %f segundos\n", elapsed_d);

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