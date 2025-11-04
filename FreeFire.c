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
void ordenarItens(Item mochila[], int qtd);
int buscaBinaria(Item mochila[], int qtd, char nomeBusca[]);

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

// Converte string para minúsculas
void toLowerStr(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

// Ordena os itens por nome (ordem alfabética)
void ordenarItens(Item mochila[], int qtd) {
    Item temp;
    for (int i = 0; i < qtd - 1; i++) {
        for (int j = i + 1; j < qtd; j++) {
            char nome1[30], nome2[30];
            strcpy(nome1, mochila[i].nome);
            strcpy(nome2, mochila[j].nome);
            toLowerStr(nome1);
            toLowerStr(nome2);

            if (strcmp(nome1, nome2) > 0) {
                temp = mochila[i];
                mochila[i] = mochila[j];
                mochila[j] = temp;
            }
        }
    }
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

    // Ordena automaticamente após inserir
    ordenarItens(mochila, *qtd);

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
    toLowerStr(nomeRemover);

    int index = buscaBinaria(mochila, *qtd, nomeRemover);
    if (index == -1) {
        printf("\nItem nao encontrado!\n");
        return;
    }

    for (int j = index; j < *qtd - 1; j++) {
        mochila[j] = mochila[j + 1];
    }
    (*qtd)--;

    printf("\nItem removido com sucesso!\n");
    listarItens(mochila, *qtd);
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

// Função de busca binária (retorna índice ou -1 se não encontrar)
int buscaBinaria(Item mochila[], int qtd, char nomeBusca[]) {
    int inicio = 0, fim = qtd - 1;

    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;

        char nomeItem[30];
        strcpy(nomeItem, mochila[meio].nome);
        toLowerStr(nomeItem);

        int cmp = strcmp(nomeItem, nomeBusca);

        if (cmp == 0) {
            return meio;
        } else if (cmp < 0) {
            inicio = meio + 1;
        } else {
            fim = meio - 1;
        }
    }
    return -1;
}

// Função para buscar um item (usa busca binária)
void buscarItem(Item mochila[], int qtd) {
    if (qtd == 0) {
        printf("\nA mochila esta vazia.\n");
        return;
    }

    char nomeBusca[30];
    printf("\nDigite o nome do item que deseja buscar: ");
    fgets(nomeBusca, sizeof(nomeBusca), stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';
    toLowerStr(nomeBusca);

    int index = buscaBinaria(mochila, qtd, nomeBusca);

    if (index != -1) {
        printf("\nItem encontrado!\n");
        printf("Nome: %s\nTipo: %s\nQuantidade: %d\n",
               mochila[index].nome, mochila[index].tipo, mochila[index].quantidade);
    } else {
        printf("\nItem nao encontrado!\n");
    }
}
