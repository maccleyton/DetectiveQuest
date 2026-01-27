#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Estrutura que representa uma sala da mansão
 * Cada sala possui um nome e dois caminhos possíveis
 */
typedef struct Sala {
    char nome[50];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

/*
 * Cria uma sala dinamicamente e inicializa seus campos
 */
Sala* criarSala(const char *nome) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    if (nova == NULL) {
        printf("Erro ao alocar memória.\n");
        exit(1);
    }

    strcpy(nova->nome, nome);
    nova->esquerda = NULL;
    nova->direita = NULL;

    return nova;
}

/*
 * Permite que o jogador explore a mansão
 */
void explorarSalas(Sala *atual) {
    char escolha;

    while (atual != NULL) {
        printf("\nVocê está em: %s\n", atual->nome);

        // Se for sala final (nó-folha)
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Não há mais caminhos. Fim da exploração.\n");
            return;
        }

        printf("Escolha um caminho:\n");
        if (atual->esquerda != NULL) printf("e - Esquerda\n");
        if (atual->direita != NULL) printf("d - Direita\n");
        printf("s - Sair\n");
        printf("> ");

        scanf(" %c", &escolha);

        if (escolha == 'e' && atual->esquerda != NULL) {
            atual = atual->esquerda;
        } else if (escolha == 'd' && atual->direita != NULL) {
            atual = atual->direita;
        } else if (escolha == 's') {
            printf("Exploração encerrada.\n");
            return;
        } else {
            printf("Opção inválida.\n");
        }
    }
}

/*
 * Função principal
 * Monta o mapa da mansão e inicia a exploração
 */
int main() {
    // Criação das salas
    Sala *hall = criarSala("Hall de Entrada");
    Sala *salaEstar = criarSala("Sala de Estar");
    Sala *cozinha = criarSala("Cozinha");
    Sala *biblioteca = criarSala("Biblioteca");
    Sala *jardim = criarSala("Jardim");
    Sala *quarto = criarSala("Quarto Secreto");

    // Montagem da árvore (mapa da mansão)
    hall->esquerda = salaEstar;
    hall->direita = cozinha;

    salaEstar->esquerda = biblioteca;
    salaEstar->direita = jardim;

    cozinha->direita = quarto;

    // Início da exploração
    explorarSalas(hall);

    return 0;
}

// Nível novato concluído com sucesso.