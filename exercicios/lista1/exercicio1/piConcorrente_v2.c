#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

double piEstimado;
pthread_mutex_t mutex;

typedef struct {
    int nthreads;
    int id;
    long int n;
} tArgs;

void * tarefa (void * arg) {
    tArgs * args = (tArgs*) arg;

    int nthreads = args->nthreads;
    long int n = args->n;
    int id = args->id;

    double resultado;
    resultado = 0;

    for (long int k = id; k < n; k += nthreads) {
        resultado += (1.0 / pow(16, k)) * (
                    4.0 / (8.0 * k + 1) - 
                    2.0 / (8.0 * k + 4) - 
                    1.0 / (8.0 * k + 5) - 
                    1.0 / (8.0 * k + 6)
                    );
    }
    
    // secao critica
    pthread_mutex_lock(&mutex);
    piEstimado += resultado;
    pthread_mutex_unlock(&mutex);
    
    pthread_exit(NULL);
}

int main(int argc, char * argv[]) {

    if (argc < 3) {
        printf("Insira o valor de n e a quantidade de threads na linha de comando\n");
        exit(-1);
    }

    long int n = atoll(argv[1]);
    int nthreads = atol(argv[2]);

    pthread_t tid[nthreads];
    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < nthreads; i++) {
        tArgs *args = (tArgs*) malloc(sizeof(tArgs));
        if (args == NULL) {
            printf("Erro ao alocar memoria\n");
            exit(-1);
        }
        args->id = i;
        args->nthreads = nthreads;
        args->n = n;

        if (pthread_create(&tid[i], NULL, tarefa, (void*) args)) {
            printf("Erro - pthread_create\n");
            exit(-1);
        }
    }

    for (int i = 0; i < nthreads; i++) {
        if (pthread_join(tid[i], NULL)) {
            printf("Erro\n");
            exit(-1);
        }
    }
    
    pthread_mutex_destroy(&mutex);
    
    printf("%f\n", piEstimado);

    return 0;
}
