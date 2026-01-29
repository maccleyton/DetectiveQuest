#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_HASH 10

/* ---------- ESTRUTURAS ---------- */

// Sala da mansão (árvore binária)
typedef struct Sala {
    char nome[50];
    char pista[100];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Nó da BST de pistas
typedef struct PistaNode {
    char pista[100];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

// Nó da tabela hash (encadeamento)
typedef struct HashNode {
    char pista[100];
    char suspeito[50];
    struct HashNode *prox;
} HashNode;

/* ---------- FUNÇÕES AUXILIARES ---------- */

// Função hash simples
int funcaoHash(const char *chave) {
    int soma = 0;
    for (int i = 0; chave[i]; i++)
        soma += chave[i];
    return soma % TAM_HASH;
}

/*
 * criarSala – cria dinamicamente um cômodo
 */
Sala* criarSala(const char *nome, const char *pista) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    strcpy(nova->pista, pista);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

/*
 * inserirPista – insere pista na BST
 */
PistaNode* inserirPista(PistaNode *raiz, const char *pista) {
    if (raiz == NULL) {
        PistaNode *novo = (PistaNode*) malloc(sizeof(PistaNode));
        strcpy(novo->pista, pista);
        novo->esquerda = NULL;
        novo->direita = NULL;
        return novo;
    }

    if (strcmp(pista, raiz->pista) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    else if (strcmp(pista, raiz->pista) > 0)
        raiz->direita = inserirPista(raiz->direita, pista);

    return raiz;
}

/*
 * inserirNaHash – associa pista a suspeito
 */
void inserirNaHash(HashNode *tabela[], const char *pista, const char *suspeito) {
    int idx = funcaoHash(pista);
    HashNode *novo = (HashNode*) malloc(sizeof(HashNode));

    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->prox = tabela[idx];
    tabela[idx] = novo;
}

/*
 * encontrarSuspeito – retorna o suspeito associado à pista
 */
char* encontrarSuspeito(HashNode *tabela[], const char *pista) {
    int idx = funcaoHash(pista);
    HashNode *atual = tabela[idx];

    while (atual) {
        if (strcmp(atual->pista, pista) == 0)
            return atual->suspeito;
        atual = atual->prox;
    }
    return NULL;
}

/*
 * explorarSalas – navegação e coleta de pistas
 */
void explorarSalas(Sala *atual, PistaNode **bstPistas, HashNode *hash[]) {
    char op;

    while (atual) {
        printf("\nVocê está em: %s\n", atual->nome);

        if (strlen(atual->pista) > 0) {
            printf("Pista encontrada: %s\n", atual->pista);
            *bstPistas = inserirPista(*bstPistas, atual->pista);
        }

        printf("\nCaminhos:\n");
        if (atual->esquerda) printf("e - Esquerda\n");
        if (atual->direita) printf("d - Direita\n");
        printf("s - Sair\n> ");

        scanf(" %c", &op);

        if (op == 'e' && atual->esquerda)
            atual = atual->esquerda;
        else if (op == 'd' && atual->direita)
            atual = atual->direita;
        else if (op == 's')
            return;
        else
            printf("Opção inválida.\n");
    }
}

/*
 * verificarSuspeitoFinal – julgamento lógico final
 */
void verificarSuspeitoFinal(PistaNode *raiz, HashNode *hash[], const char *acusado, int *contador) {
    if (!raiz) return;

    verificarSuspeitoFinal(raiz->esquerda, hash, acusado, contador);

    char *suspeito = encontrarSuspeito(hash, raiz->pista);
    if (suspeito && strcmp(suspeito, acusado) == 0)
        (*contador)++;

    verificarSuspeitoFinal(raiz->direita, hash, acusado, contador);
}

/*
 * Exibe pistas em ordem alfabética
 */
void exibirPistas(PistaNode *raiz) {
    if (!raiz) return;
    exibirPistas(raiz->esquerda);
    printf("- %s\n", raiz->pista);
    exibirPistas(raiz->direita);
}

/* ---------- MAIN ---------- */

int main() {
    // Tabela hash
    HashNode *hash[TAM_HASH] = {NULL};

    // Associação pista -> suspeito
    inserirNaHash(hash, "Pegadas no tapete", "Mordomo");
    inserirNaHash(hash, "Faca suja de sangue", "Cozinheiro");
    inserirNaHash(hash, "Livro fora do lugar", "Bibliotecária");
    inserirNaHash(hash, "Carta escondida", "Mordomo");

    // Mansão
    Sala *hall = criarSala("Hall", "Pegadas no tapete");
    Sala *cozinha = criarSala("Cozinha", "Faca suja de sangue");
    Sala *biblioteca = criarSala("Biblioteca", "Livro fora do lugar");
    Sala *quarto = criarSala("Quarto Secreto", "Carta escondida");

    hall->esquerda = biblioteca;
    hall->direita = cozinha;
    cozinha->direita = quarto;

    PistaNode *bstPistas = NULL;

    explorarSalas(hall, &bstPistas, hash);

    printf("\nPistas coletadas:\n");
    exibirPistas(bstPistas);

    char acusado[50];
    printf("\nQuem é o culpado? ");
    scanf(" %[^\n]", acusado);

    int provas = 0;
    verificarSuspeitoFinal(bstPistas, hash, acusado, &provas);

    if (provas >= 2)
        printf("\nAcusação aceita! %s é o culpado.\n", acusado);
    else
        printf("\nProvas insuficientes. %s não pode ser condenado.\n", acusado);

    return 0;
}



// Nível mestre concluído com sucesso.