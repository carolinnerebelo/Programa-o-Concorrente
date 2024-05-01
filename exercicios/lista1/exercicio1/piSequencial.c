#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double bailey_borwein(long int n);

int main(int argc, char * argv[]) {

    if (argc < 2) {
        printf("Insira o valor de n na linha de comando\n");
        exit(-1);
    }

    double resultado;
    long int n = atoll(argv[1]);
    resultado = bailey_borwein(n);

    printf("%.15f\n", resultado);

    return 0;
}

double bailey_borwein(long int n) {
    double resultado = 0.0;

    for (int k = 0; k < n; k++) {
        resultado += (1.0 / pow(16, k)) * (
                    4.0 / (8.0 * k + 1) - 
                    2.0 / (8.0 * k + 4) - 
                    1.0 / (8.0 * k + 5) - 
                    1.0 / (8.0 * k + 6)
                    );
    }

    return resultado;
}

