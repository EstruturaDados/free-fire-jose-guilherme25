// Importando bibliotecas
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_ITENS 10

// Estrutura para representar um item na mochila
typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
} Item;

// Declaração das funções
void inserirItem(Item mochila[], int *qtd);
void removerItem(Item mochila[], int *qtd);
void listarItens(Item mochila[], int qtd);
void buscarItem(Item mochila[], int qtd);

int main() {
    Item mochila[MAX_ITENS];
    int qtd = 0;
    int opcao;

    do {
        printf("\n======== MOCHILA DE LOOT ========\n");
        printf("1. Adicionar item\n");
        printf("2. Remover item\n");
        printf("3. Listar itens\n");
        printf("4. Buscar item\n");
        printf("0. Sair\n");
        printf("=================================\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); // Limpa o buffer

        switch (opcao) {
            case 1:
                inserirItem(mochila, &qtd);
                break;
            case 2:
                removerItem(mochila, &qtd);
                break;
            case 3:
                listarItens(mochila, qtd);
                break;
            case 4:
                buscarItem(mochila, qtd);
                break;
            case 0:
                printf("\nSaindo do sistema...\n");
                break;
            default:
                printf("\nOpcao invalida! Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}

// Função para adicionar um novo item
void inserirItem(Item mochila[], int *qtd) {
    if (*qtd >= MAX_ITENS) {
        printf("\nA mochila esta cheia! Nao e possivel adicionar mais itens.\n");
        return;
    }

    Item novoItem;

    printf("\n--- Cadastro de novo item ---\n");
    printf("Nome do item: ");
    fgets(novoItem.nome, sizeof(novoItem.nome), stdin);
    novoItem.nome[strcspn(novoItem.nome, "\n")] = '\0';

    printf("Tipo do item (arma, municao, cura, etc): ");
    fgets(novoItem.tipo, sizeof(novoItem.tipo), stdin);
    novoItem.tipo[strcspn(novoItem.tipo, "\n")] = '\0';

    printf("Quantidade: ");
    scanf("%d", &novoItem.quantidade);
    getchar(); // Limpa o buffer

    mochila[*qtd] = novoItem;
    (*qtd)++;

    printf("\nItem adicionado com sucesso!\n");
    listarItens(mochila, *qtd);
}

// Função para remover um item (ignora maiúsculas/minúsculas)
void removerItem(Item mochila[], int *qtd) {
    if (*qtd == 0) {
        printf("\nA mochila esta vazia. Nenhum item para remover.\n");
        return;
    }

    char nomeRemover[30];
    printf("\nDigite o nome do item que deseja remover: ");
    fgets(nomeRemover, sizeof(nomeRemover), stdin);
    nomeRemover[strcspn(nomeRemover, "\n")] = '\0';

    for (int i = 0; nomeRemover[i]; i++) {
        nomeRemover[i] = tolower(nomeRemover[i]);
    }

    for (int i = 0; i < *qtd; i++) {
        char nomeItem[30];
        strcpy(nomeItem, mochila[i].nome);

        for (int j = 0; nomeItem[j]; j++) {
            nomeItem[j] = tolower(nomeItem[j]);
        }

        if (strcmp(nomeItem, nomeRemover) == 0) {
            for (int j = i; j < *qtd - 1; j++) {
                mochila[j] = mochila[j + 1];
            }
            (*qtd)--;

            printf("\nItem removido com sucesso!\n");
            listarItens(mochila, *qtd);
            return;
        }
    }

    printf("\nItem nao encontrado!\n");
}

// Função para listar todos os itens
void listarItens(Item mochila[], int qtd) {
    if (qtd == 0) {
        printf("\nA mochila esta vazia.\n");
        return;
    }

    printf("\n--- Itens na mochila: %d/%d ---\n", qtd, MAX_ITENS);
    for (int i = 0; i < qtd; i++) {
        printf("%d) Nome: %s | Tipo: %s | Quantidade: %d\n",
               i + 1, mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
    }
}

// Função para buscar um item (ignora maiúsculas/minúsculas)
void buscarItem(Item mochila[], int qtd) {
    if (qtd == 0) {
        printf("\nA mochila esta vazia.\n");
        return;
    }

    char nomeBusca[30];
    printf("\nDigite o nome do item que deseja buscar: ");
    fgets(nomeBusca, sizeof(nomeBusca), stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

    for (int i = 0; nomeBusca[i]; i++) {
        nomeBusca[i] = tolower(nomeBusca[i]);
    }

    for (int i = 0; i < qtd; i++) {
        char nomeItem[30];
        strcpy(nomeItem, mochila[i].nome);

        for (int j = 0; nomeItem[j]; j++) {
            nomeItem[j] = tolower(nomeItem[j]);
        }

        if (strcmp(nomeItem, nomeBusca) == 0) {
            printf("\nItem encontrado!\n");
            printf("Nome: %s\nTipo: %s\nQuantidade: %d\n",
                   mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
            return;
        }
    }

    printf("\nItem nao encontrado!\n");
}
