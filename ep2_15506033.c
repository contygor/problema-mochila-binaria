#include <stdio.h>
#include <stdlib.h>

int quantidadeLinhas (FILE *fin) {
    int peso, valor;
    int linha = 0;

    while(fscanf(fin, "%d %d", &peso, &valor) == 2) {
        linha++;
    }

    rewind(fin);
    return linha;
}

void carregarArranjos (FILE *fin, int *peso, int *valor, int n) {
    for(int i = 0; i < n; i++) {
        fscanf(fin, "%d %d", &peso[i], &valor[i]);
    }
}

int main (int argc, char *argv[]) {
    if(argc != 3) {
        printf("A chamada do executável deve ser %s <nome_arquivo_entrada.txt> <nome_arquivo_saida.txt>\n", argv[0]);
        exit(1);
    }

    FILE *fin = fopen(argv[1], "r");
    if(fin == NULL) {
        printf("Erro ao abrir o arquivo .txt de entrada %s\n", argv[1]);
        exit(1);
    }

    int n = quantidadeLinhas(fin);
    int *peso;
    int *valor;
    peso = calloc(n, sizeof(int));
    valor = calloc(n, sizeof(int));
    if (peso == NULL || valor == NULL) {
        printf("Erro ao alocar memória para os arranjos\n");
        exit(1);
    }

    carregarArranjos(fin, &peso, &valor, n);
    
    fclose(fin);
    free(peso);
    free(valor);
    return 0;
}

