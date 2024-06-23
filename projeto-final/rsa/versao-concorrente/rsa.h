#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <locale.h>
#include "timer.h"

// #define TESTE

// Estrutura para passar dados para as threads
typedef struct {
    long long *args;
    long long chave;
    long long n;
    int inicio;
    int fim;
} ThreadArgs;

long long gcd(long long a, long long b);
long long achad(long long e, long long phi);
long long mod_exp(long long base, long long exp, long long mod);
int ehPrimo(long long int n);
long long gera_primo();
void gera_chave_rsa(long long *n, long long *e, long long *d);
void *criptografa(void *arg);
void *descriptografa(void *arg);
void string_to_numbers(char *str, long long *numeros);
void numbers_to_string(long long *numeros, int len, char *str);