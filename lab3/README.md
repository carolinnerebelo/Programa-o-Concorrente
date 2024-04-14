# Multiplicação de matrizes
Programa concorrente para a multiplicação de matrizes.

## Conteúdo
- "multMatriz.c": programa principal que multiplica matrizes de forma concorrente.
- "multMatrizSeq.c": programa sequencial que multiplica matrizes para fins de teste.
- "geraMatriz.c": programa que gera elementos aleatórios para a matriz e os armazena em um arquivo binário.
- Arquivos binários usados para testes: A500 e B500 sendo duas matrizes A e B de dimensão 500 x 500; análogo para A1000 e B1000 e A2000 e B2000.

## Execução
- multMatriz deve receber como argumentos na linha de comando: o arquivo binário que contém a matriz A, o arquivo binário que contém a matriz B, um nome para o arquivo de saída e a quantidade de threads desejada.
- multMatrizSeq deve receber como argumentos na linha de comando: o arquivo binário que contém a matriz A, o arquivo binário que contém a matriz B e um nome para o arquivo de saída.
- geraMatriz deve receber como argumentos na linha de comando: a quantidade de linhas da matriz, a quantidade de colunas da matriz e o nome do arquivo de saída.
