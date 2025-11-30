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

typedef struct {
    double pesoUsado;
    double valorCarregado;
    int numObjetos;
} ResultadoGuloso;

/* Declaração antecipada para liberarLista */
void liberarLista(Objeto *inicio);

Objeto* lerArquivo (FILE *file, int *tamanho, int *capacidade) {
    int peso, valor;
    Objeto *inicio = NULL;
    Objeto *atual = NULL;
    int n;

    /* Lê a primeira linha: capacidade e número de objetos */
    if (fscanf(file, "%d %d", capacidade, &n) != 2) {
        return NULL;
    }

    while (fscanf(file, "%d %d", &peso, &valor) == 2) {
        Objeto *novo = (Objeto*) malloc(sizeof(Objeto));
        if (novo == NULL) {
            liberarLista(inicio);
            return NULL;
        }
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

        (*tamanho)++;
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
            else {
                /* Empate na razão: prefere maior valor */
                return a->valor - b->valor;
            }
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

Objeto* intercalar(Objeto *esquerda, Objeto *direita, Criterio criterio) {
    Objeto *resultado = NULL;

    if (esquerda == NULL)
        return direita;
    if (direita == NULL)
        return esquerda;

    /* Para PESO: menor peso primeiro; para VALOR e RAZAO: maior primeiro */
    if (criterio == PESO) {
        if (comparar(esquerda, direita, criterio) <= 0) {
            resultado = esquerda;
            resultado->prox = intercalar(esquerda->prox, direita, criterio);
        } else {
            resultado = direita;
            resultado->prox = intercalar(esquerda, direita->prox, criterio);
        }
    } else {
        /* Para VALOR e RAZAO_PESO_VALOR: ordem decrescente (maior primeiro) */
        if (comparar(esquerda, direita, criterio) >= 0) {
            resultado = esquerda;
            resultado->prox = intercalar(esquerda->prox, direita, criterio);
        } else {
            resultado = direita;
            resultado->prox = intercalar(esquerda, direita->prox, criterio);
        }
    }
    return resultado;
}

void mergeSort(Objeto **inicio, Criterio criterio) {
    Objeto *cabeca = *inicio;
    Objeto *esquerda;
    Objeto *direita;

    if (cabeca == NULL || cabeca->prox == NULL)
        return;

    dividirLista(cabeca, &esquerda, &direita);

    mergeSort(&esquerda, criterio);
    mergeSort(&direita, criterio);

    *inicio = intercalar(esquerda, direita, criterio);
}

Objeto* copiarLista(Objeto *original) {
    if (original == NULL)
        return NULL;

    Objeto *novaLista = NULL;
    Objeto *atual = NULL;
    Objeto *orig = original;

    while (orig != NULL) {
        Objeto *novo = (Objeto*) malloc(sizeof(Objeto));
        if (novo == NULL) {
            liberarLista(novaLista);
            return NULL;
        }
        novo->peso = orig->peso;
        novo->valor = orig->valor;
        novo->prox = NULL;

        if (novaLista == NULL) {
            novaLista = novo;
            atual = novo;
        } else {
            atual->prox = novo;
            atual = novo;
        }
        orig = orig->prox;
    }

    return novaLista;
}

ResultadoGuloso guloso(Objeto *lista, int capacidade, Criterio criterio) {
    ResultadoGuloso resultado;
    resultado.pesoUsado = 0;
    resultado.valorCarregado = 0;
    resultado.numObjetos = 0;

    /* Copia a lista para não modificar a original */
    Objeto *copia = copiarLista(lista);

    /* Ordena a cópia pelo critério */
    mergeSort(&copia, criterio);

    /* Aplica o algoritmo guloso */
    Objeto *atual = copia;
    int capacidadeRestante = capacidade;

    while (atual != NULL) {
        if (atual->peso <= capacidadeRestante) {
            capacidadeRestante -= atual->peso;
            resultado.pesoUsado += atual->peso;
            resultado.valorCarregado += atual->valor;
            resultado.numObjetos++;
        }
        atual = atual->prox;
    }

    /* Libera a cópia */
    liberarLista(copia);

    return resultado;
}

const char* nomeCriterio(Criterio criterio) {
    switch(criterio) {
        case PESO:
            return "Menor Peso";
        case VALOR:
            return "Maior Valor";
        case RAZAO_PESO_VALOR:
            return "Maior Valor Dividido pelo Peso";
        default:
            return "Desconhecido";
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

    int tamanho = 0;
    int capacidade = 0;
    Objeto *lista = lerArquivo(fin, &tamanho, &capacidade);
    fclose(fin);

    FILE *fout = fopen(argv[2], "w");
    if(fout == NULL) {
        printf("Erro ao abrir o arquivo .txt de saída %s\n", argv[2]);
        liberarLista(lista);
        exit(1);
    }

    Criterio criterios[] = {PESO, VALOR, RAZAO_PESO_VALOR};
    int numCriterios = 3;

    for (int i = 0; i < numCriterios; i++) {
        ResultadoGuloso resultado = guloso(lista, capacidade, criterios[i]);

        fprintf(fout, "Testando Criterio de Selecao: '%s'\n", nomeCriterio(criterios[i]));
        fprintf(fout, "Peso maximo:\t  %.1f\n", (double)capacidade);
        fprintf(fout, "Peso usado:\t  %.1f\n", resultado.pesoUsado);
        fprintf(fout, "Valor carregado:  %.1f\n", resultado.valorCarregado);
        fprintf(fout, "Numero de objetos:\t%d\n", resultado.numObjetos);

        if (i < numCriterios - 1) {
            fprintf(fout, "\n");
        }
    }

    fclose(fout);
    liberarLista(lista);
    return 0;
}

