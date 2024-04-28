/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

// versão sequencial

#include <stdio.h>
#include <stdlib.h>

double piSequencial (long long n) {
    double soma = 0.0, fator = 1.0;
    long long i;
    
    for (i = 0; i < n; i++) {
        soma = soma + fator/(2*i+1);
        fator = -fator;
    }
    
    return 4.0 * soma;
}

int main(int argc, char * argv[]) {
    if (argc < 2) {
        printf("Insira o valor de n: %s <n>\n", argv[0]);
        exit(-1);
    }
    
    long long n = atoll(argv[1]);
    double pi_estimado;
    
    pi_estimado = piSequencial(n);
    
    printf("Valor estimado de pi: %f\n", pi_estimado);
    
    return 0;
}

