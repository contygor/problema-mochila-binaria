#include <stdio.h>
#include <stdlib.h>

typedef struct Objeto {
    int peso;
    int valor;
    struct Objeto *prox;
} Objeto;

typedef enum {
    PESO,
    VALOR,
    RAZAO_PESO_VALOR
} Criterio;

Objeto* lerArquivo (FILE *file, int *tamanho) {
    int peso, valor;
    Objeto *inicio = NULL;
    Objeto *atual = NULL;

    while (fscanf(file, "%d %d", &peso, &valor) == 2) {
        Objeto *novo = (Objeto*) malloc(sizeof(Objeto));
        novo->peso = peso;
        novo->valor = valor;
        novo->prox = NULL;

        if (inicio == NULL) {
            inicio = novo;
            atual = novo;
        } else {
            atual->prox = novo;
            atual = novo;
        }

        *tamanho++;
    }

    return inicio;
} 

void liberarLista (Objeto *inicio) {
    Objeto *atual = inicio;
    Objeto *proximo;

    while (atual != NULL) {
        proximo = atual->prox;
        free(atual);
        atual = proximo;
    }
}

void imprimirLista(Objeto *ini) {
    int i = 1;
    while (ini != NULL) {
        printf("Objeto %d -> Peso: %d | Valor: %d\n", i, ini->peso, ini->valor);
        ini = ini->prox;
        i++;
    }
}

int comparar (Objeto *a, Objeto *b, Criterio criterio) {
    switch(criterio) {
        case PESO:
            return a->peso - b->peso;
        case VALOR:
            return a->valor - b->valor;
        case RAZAO_PESO_VALOR: {
            double razaoA = (double)a->valor / a->peso;
            double razaoB = (double)b->valor / b->peso;
            if (razaoA < razaoB) return -1;
            else if (razaoA > razaoB) return 1;
            else return 0;
        }
        default:
            return 0;
    }
}

void dividirLista (Objeto *inicio, Objeto **esquerda, Objeto **direita) {
    Objeto *rapido;
    Objeto *lento;
    lento = inicio;
    rapido = inicio->prox;

    while (rapido != NULL) {
        rapido = rapido->prox;
        if (rapido != NULL) {
            lento = lento->prox;
            rapido = rapido->prox;
        }
    }

    *esquerda = inicio;
    *direita = lento->prox;
    lento->prox = NULL;
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

    int tamanho = 0;
    Objeto *lista = lerArquivo(fin, &tamanho);

    imprimirLista(lista);

    liberarLista(lista);
    fclose(fin);
    return 0;
}

