#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Estrutura que representa uma sala da mansão
 */
typedef struct Sala {
    char nome[50];
    char pista[100];        // string vazia significa "sem pista"
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

/*
 * Estrutura da árvore BST de pistas
 */
typedef struct PistaNode {
    char pista[100];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

/*
 * Cria dinamicamente uma sala com nome e pista (opcional)
 */
Sala* criarSala(const char *nome, const char *pista) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    if (nova == NULL) {
        printf("Erro de alocação de memória.\n");
        exit(1);
    }

    strcpy(nova->nome, nome);
    strcpy(nova->pista, pista);
    nova->esquerda = NULL;
    nova->direita = NULL;

    return nova;
}

/*
 * Insere uma pista na BST de pistas coletadas
 */
PistaNode* inserirPista(PistaNode *raiz, const char *pista) {
    if (raiz == NULL) {
        PistaNode *novo = (PistaNode*) malloc(sizeof(PistaNode));
        strcpy(novo->pista, pista);
        novo->esquerda = NULL;
        novo->direita = NULL;
        return novo;
    }

    if (strcmp(pista, raiz->pista) < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    } else if (strcmp(pista, raiz->pista) > 0) {
        raiz->direita = inserirPista(raiz->direita, pista);
    }

    return raiz;
}

/*
 * Exibe todas as pistas coletadas em ordem alfabética
 */
void exibirPistas(PistaNode *raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("- %s\n", raiz->pista);
        exibirPistas(raiz->direita);
    }
}

/*
 * Controla a exploração da mansão e a coleta de pistas
 */
void explorarSalasComPistas(Sala *atual, PistaNode **pistasColetadas) {
    char escolha;

    while (atual != NULL) {
        printf("\nVocê está em: %s\n", atual->nome);

        // Coleta da pista (se existir)
        if (strlen(atual->pista) > 0) {
            printf("Pista encontrada: %s\n", atual->pista);
            *pistasColetadas = inserirPista(*pistasColetadas, atual->pista);
        }

        printf("\nCaminhos disponíveis:\n");
        if (atual->esquerda != NULL) printf("e - Esquerda\n");
        if (atual->direita != NULL) printf("d - Direita\n");
        printf("s - Sair\n");
        printf("> ");

        scanf(" %c", &escolha);

        if (escolha == 'e' && atual->esquerda != NULL) {
            atual = atual->esquerda;
        } 
        else if (escolha == 'd' && atual->direita != NULL) {
            atual = atual->direita;
        } 
        else if (escolha == 's') {
            printf("\nExploração encerrada.\n");
            return;
        } 
        else {
            printf("Opção inválida.\n");
        }
    }
}

/*
 * Função principal
 * Monta a mansão e inicia a investigação
 */
int main() {
    // Criação das salas com pistas
    Sala *hall = criarSala("Hall de Entrada", "Pegadas recentes no tapete");
    Sala *salaEstar = criarSala("Sala de Estar", "Almofada rasgada com fios de tecido");
    Sala *cozinha = criarSala("Cozinha", "Faca com marcas suspeitas");
    Sala *biblioteca = criarSala("Biblioteca", "Livro fora do lugar");
    Sala *jardim = criarSala("Jardim", "Terra revirada perto da cerca");
    Sala *quarto = criarSala("Quarto Secreto", "Carta escondida sob o assoalho");

    // Montagem do mapa (árvore binária)
    hall->esquerda = salaEstar;
    hall->direita = cozinha;

    salaEstar->esquerda = biblioteca;
    salaEstar->direita = jardim;

    cozinha->direita = quarto;

    // Árvore BST de pistas
    PistaNode *pistasColetadas = NULL;

    // Início da exploração
    explorarSalasComPistas(hall, &pistasColetadas);

    // Exibição final das pistas
    printf("\nPistas coletadas (ordem alfabética):\n");
    exibirPistas(pistasColetadas);

    return 0;
}


// Nível aventureiro concluído com sucesso.