/*
 * ACH2002 - Introdução à Análise de Algoritmos
 * EP2 - Algoritmo Guloso para o Problema da Mochila Binária
 * Docente: Ariane Machado Lima
 * 
 * Nome: Ygor Araujo da Silva
 * Nusp: 15506033
 * 
 * Obs: Não realizei nenhum tratamento de erro caso a entrada fornecida pelo usuário contenham mais de dois parâmetros por linha
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

/*
Struct criada com os atributos peso, valor e um ponteiro para o próximo objeto da lista construída a partir do arquivo de entrada.
*/
typedef struct Objeto {
    int peso;
    int valor;
    struct Objeto *prox;
} Objeto;

/*
Struct criada para representar a lista de objetos, contendo um ponteiro para o primeiro objeto da lista, evitando 
a perda da referência ao início da lista ao manipular os objetos durante o algoritmo de ordenação mergeSort.
*/ 
typedef struct Lista {
    Objeto *inicio;
} Lista;

/*
Enum utilizado para deentradair qual o critério de ordenação a ser utilizado no algoritmo guloso durante a ordenação da lista de objetos.
*/
typedef enum {
    PESO,
    VALOR,
    RAZAO_PESO_VALOR
} Criterio;

/*
Função que lê o arquivo de entrada e constrói a lista de objetos.
*/
Lista lerArquivo (FILE *file, int *capacidade, int *numeroObjetos) {
    int peso, valor;
    Objeto *inicio = NULL;
    Objeto *atual = NULL;

    //Lê a capacidade da mochila e o número de objetos da primeira linha do arquivo de entrada
    fscanf(file, "%d %d", capacidade, numeroObjetos);
    //Lê os objetos do arquivo presentes nas linhas subsequentes e os adiciona à lista
    for (int i = 0; i < *numeroObjetos; i++) {
        fscanf(file, "%d %d", &peso, &valor);
        Objeto *novo = (Objeto*) malloc(sizeof(Objeto));
        novo->peso = peso;
        novo->valor = valor;
        novo->prox = NULL;

        /*
        Se o inicio é NULL, significa que a lista está vazia, então o novo objeto se torna o início da lista.
        */
        if (inicio == NULL) {
            inicio = novo;
            atual = novo;
        } else {
            /*
            Adiciona o novo objeto ao entradaal da lista construída até a iteração atual.
            */
            atual->prox = novo;
            atual = novo;
        }
    }

    /*
    Cria uma lista do tipo struct Lista, atribui o objeto inicial à cabeça da lista e retorna a lista em seguida
    */
    Lista lista;
    lista.inicio = inicio;
    return lista;
} 

/*
Função que libera a memória alocada para a lista de objetos
*/
void liberarLista (Objeto *inicio) {
    Objeto *atual = inicio;
    Objeto *proximo;

    while (atual != NULL) {
        proximo = atual->prox;
        free(atual);
        atual = proximo;
    }
}

/*
Função auxiliar que eu adicionei para imprimir a lista de objetos e verificar se estava de acordo com o Caso de Teste fornecido
*/
void imprimirLista(Objeto *ini) {
    int i = 1;
    printf("Objeto x -> (<peso>, <valor>)\n");
    while (ini != NULL) {
        printf("Objeto %d -> (%d, %d)\n", i, ini->peso, ini->valor);
        ini = ini->prox;
        i++;
    }
    printf("\n");
}

/*
Função auxiliar para comparar dois inteiros
*/
int comparaInt (int a, int b) {
    if (a > b) return 1;
    if (a < b) return -1;
    return 0;
}

/*
Função auxiliar para comparar dois doubles
*/
double comparaDouble (double a, double b) {
    if (a > b) return 1.0;
    if (a < b) return -1.0;
    return 0.0;
}

/*
Função que recebe dois objetos e o critério de comparação, retornando 0 se forem iguais, 1 se o primeiro objeto deve vir depois do segundo
e -1 se o primeiro objeto deve vir antes do segundo
*/
int comparar (Objeto *a, Objeto *b, Criterio criterio) {
    int resultado;
    switch(criterio) {
        case PESO:
            resultado = comparaInt(a->peso, b->peso); //menor peso primeiro
            if (resultado != 0) {
                return resultado;
            } else {
                return comparaInt(b->valor, a->valor); //maior valor primeiro
            }
        case VALOR:
            resultado = comparaInt(b->valor, a->valor); //maior valor primeiro
            if (resultado != 0) {
                return resultado;
            } else {
                return comparaInt(a->peso, b->peso); //menor peso primeiro
            }
        case RAZAO_PESO_VALOR: {
            double razaoA = (double)a->valor / (double)a->peso;
            double razaoB = (double)b->valor / (double)b->peso;

            resultado = comparaDouble(razaoB, razaoA); //maior razão primeiro
            if (resultado != 0) {
                return resultado;
            } else {
                return comparaInt(b->peso, a->peso); //maior peso primeiro
            }
        }
        default:
            return 0;
    }
}

/*
Função que divide a lista ligado em duas metades para o funcionamento correto do mergeSort
*/
void dividirLista (Objeto *inicio, Objeto **esquerda, Objeto **direita) {
    Objeto *rapido;
    Objeto *lento;
    lento = inicio;
    rapido = inicio->prox;

    /*
    Este laço avança o ponteiro "rápido" duas vezes mais rápido que o ponteiro "lento". Assim, quando o ponteiro "lento" alcançar o 
    o meio da lista, o ponteiro "rápido" estará no entradaal da lista.
    */
    while (rapido != NULL) {
        rapido = rapido->prox;
        if (rapido != NULL) {
            lento = lento->prox;
            rapido = rapido->prox;
        }
    }

    /*
    A lista da esquerda continuará a partir do ponteiro inicial, porém a lista da direita iniciará a partir do próximo nó do ponteiro lento,
    que seguirá até o entradaal da lista ligada original. Por fim, o próximo nó do ponteiro lento é deentradaido como NULL para separar a lista ligado
    ao meio de fato.
    */
    *esquerda = inicio;
    *direita = lento->prox;
    lento->prox = NULL;
}

/*
Função que concatena as duas listas ligadas, esquerda e direita, conforme o critério deentradaido
*/
Objeto* merge(Objeto *esquerda, Objeto *direita, Criterio criterio) {
    if (esquerda == NULL) return direita;
    if (direita == NULL) return esquerda;

    Objeto *resultado = NULL;

    if(comparar(esquerda, direita, criterio) <= 0) {
        /*
        Caso a função comparar retorne -1 ou 0, significa que o objeto da lista esquerda deve vir antes do objeto da lista direita
        OBS: não é porque um objeto deve vir antes, que ele necessariamente é menor que o outro, pois depende do critério de comparação,
        tendo em vista que para o critério de maior valor, por exemplo, o objeto com um maior valor é deentradaido como "menor" que o outro
        para que a lista seja ordenada de forma decrescente. Essa inversão ocorre dentro da lógica da função comparar.
        */
        resultado = esquerda;
        resultado->prox = merge(esquerda->prox, direita, criterio);
    } else {
        /*
        O objeto da lista direita deve vir antes do objeto da lista esquerda
        */
        resultado = direita;
        resultado->prox = merge(esquerda, direita->prox, criterio);
    }
    
    /*
    Retorna o ponteiro para o início da lista concatenada
    */
    return resultado;
}

/*
Função de ordenação do mergeSort que recebe como parâmetro a cabeça da lista ligada e o critério de ordenação deentradaido no enum Criterio
*/
Objeto* mergeSort(Objeto *lista, Criterio criterio) {
    if (lista == NULL || lista->prox == NULL) {
        return lista;
    }

    /*
    Cria dois ponteiros locais para armazenar as duas metades da lista que será dividida
    */
    Objeto *esquerda = NULL;
    Objeto *direita = NULL;

    dividirLista(lista, &esquerda, &direita);

    esquerda = mergeSort(esquerda, criterio);
    direita = mergeSort(direita, criterio);

    /*
    Retorna a lista ordenada de cada lado concatenado e segue a recursão até que toda a lista esteja ordenada
    */
    return merge(esquerda, direita, criterio);
}

/*
Função principal do método guloso que recebe como parâmetro a struct Lista, uma cópia da capacidade máxima da mochila, o critério 
de ordenação e o arquivo de saída para escrever os resultados.
*/
void preencherMochila(Lista *lista, int capacidade, Criterio criterio, FILE *fout) {
    int capacidadeRestante = capacidade;
    int objetosAdicionados = 0;
    int valorAdicionado = 0;

    /*
    Ordena a lista de objetos e a cabeça da lista é atualizada para o início da lista ordenada
    */
    lista->inicio = mergeSort(lista->inicio, criterio);
    Objeto *atual = lista->inicio;

    printf("Processando Método Guloso pelo critério: %s\n", criterio == PESO ? "Menor Peso" : criterio == VALOR ? "Maior Valor" : "Maior Valor Dividido pelo Peso");

    while(atual != NULL && capacidadeRestante > 0) {
        if(atual->peso <= capacidadeRestante) {
            if(criterio == RAZAO_PESO_VALOR) {
                double razao = (double)atual->valor / (double)atual->peso;
                printf("Adicionando objeto com peso %d, valor %d e razão %.3f na mochila.\n", atual->peso, atual->valor, razao);
            } else {
                printf("Adicionando objeto com peso %d e valor %d na mochila.\n", atual->peso, atual->valor);
            }
            capacidadeRestante -= atual->peso;
            objetosAdicionados++;
            valorAdicionado += atual->valor;
            atual = atual->prox;
        } else {
            break;
        }
    }
    printf("\n");

    /*
    Escreve os resultados no arquivo de saída conforme o formato solicitado no enunciado do trabalho
    */
    fprintf(fout, "Testando critério de seleção: %s", criterio == PESO ? "'Menor Peso'\n" : criterio == VALOR ? "'Maior Valor'\n" : "'Maior Valor Dividido pelo Peso'\n");
    fprintf(fout, "Peso máximo: %d\n", capacidade);
    fprintf(fout, "Peso usado: %d\n", capacidade - capacidadeRestante);
    fprintf(fout, "Valor carregado: %d\n", valorAdicionado);
    fprintf(fout, "Número de objetos: %d\n\n", objetosAdicionados);

    return;
}

int main (int argc, char *argv[]) {
    /*
    Traduz a saída para o idioma português, permitindo uso de caracteres especiais
    */
    setlocale(LC_ALL, "Portuguese");

    /*
    Valida a quantidade de argumentos passados na linha de comando
    */
    if(argc != 3) {
        printf("A chamada do executável deve ser %s <nome_arquivo_entrada.txt> <nome_arquivo_saida.txt>\n", argv[0]);
        exit(1);
    }

    /*
    Abre o arquivo de entrada para leitura
    */
    FILE *entrada = fopen(argv[1], "r");
    if(entrada == NULL) {
        printf("Erro ao abrir o arquivo .txt de entrada %s\n", argv[1]);
        exit(1);
    }

    /*
    Abre o arquivo de saída para escrita
    */
    FILE *saida = fopen(argv[2], "w");
    if (saida == NULL) {
        printf("Erro ao abrir o arquivo de saída %s\n", argv[2]);
        exit(1);
    }

    printf("Arquivo de entrada \"%s\" lido com sucesso.\n\n", argv[1]);

    int capacidade = 0;
    int numeroObjetos = 0;
    Lista lista = lerArquivo(entrada, &capacidade, &numeroObjetos);

    /*
    Função auxiliar que eu utilizei para verificar se a lista estava sendo construída corretamente a partir do arquivo de entrada
    */
    //imprimirLista(lista.inicio);

    /*
    Execução do método guloso em si para cada um dos três critérios de ordenação
    OBS: como nenhum objeto dentro da lista ligada é removido, eu não criei uma lista diferente para executar cada critério, apenas reutilizei
    e reordenei a mesma lista em cada chamada. Por conta, disso eu criei a struct Lista para manter a referência ao início da lista ligada, pois
    percebi que não conseguiria fazer isso apenas com um ponteiro para o objeto do início da lista ligada, que estava se perdendo a cada
    ordenação e ignorando a existência dos outros objetos.
    */
    preencherMochila(&lista, capacidade, PESO, saida);
    preencherMochila(&lista, capacidade, VALOR, saida);
    preencherMochila(&lista, capacidade, RAZAO_PESO_VALOR, saida);
    printf("Resultados escritos no arquivo de saída \"%s\"\n", argv[2]);

    /*
    Libera a memória alocada para a lista de objetos e fecha os arquivos de entrada e saída
    */
    liberarLista(lista.inicio);
    fclose(entrada);
    fclose(saida);
    return 0;
}

