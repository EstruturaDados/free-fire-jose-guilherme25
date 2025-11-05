/*
 * Sistema: PLANO DE FUGA - CÓDIGO DA ILHA (NÍVEL MESTRE)
 * Limite de itens: 10
 *
 * Funcionalidades:
 * - Cadastro de componentes (nome, tipo, prioridade)
 * - Descartar componente (opção 2)
 * - Listar componentes (inventário)
 * - Painel secundário "Organizar Mochila" com 3 algoritmos:
 *     1) Bubble Sort por nome (string, case-insensitive)
 *     2) Insertion Sort por tipo (string, case-insensitive)
 *     3) Selection Sort por prioridade (int, desc)
 * - Busca binária por nome (apenas após ordenação por nome) - case-insensitive
 * - Contagem de comparações e medição de tempo com clock()
 *
 * Observações:
 * - Todas as entradas de string usam fgets + remoção de newline.
 * - Comparações de strings são feitas em modo case-insensitive com strcmpIgnoreCase.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_ITENS 10
#define MAX_NOME 30
#define MAX_TIPO 20

/* --- Struct do componente --- */
typedef struct {
    char nome[MAX_NOME];
    char tipo[MAX_TIPO];
    int prioridade; // 1..10
} Componente;

/* --- Protótipos das funções --- */
void trimNewline(char *s);
void lerString(char *dest, int tamanho);
void mostrarHeader(int qtd, int limite, int sortedBy);
void mostrarComponentes(Componente comps[], int n);
int strcmpIgnoreCase(const char *a, const char *b);
void copiarComponentes(Componente dest[], Componente src[], int n);

/* Algoritmos de ordenação — retornam número de comparações */
long long bubbleSortNome(Componente v[], int n);       // por nome (string) - bubble
long long insertionSortTipo(Componente v[], int n);    // por tipo (string) - insertion
long long selectionSortPrioridade(Componente v[], int n); // por prioridade (int) - selection (desc)

/* Busca binária por nome (case-insensitive). Recebe ponteiro para comparações */
int buscaBinariaPorNome(Componente v[], int n, const char chave[], long long *comparacoes);

/* Medição de tempo: executa algoritmo em cópia e devolve tempo; também devolve comparações */
double medirTempo(long long (*algoritmo)(Componente[], int), Componente arr[], int n, long long *comparacoes);

/* --- Implementações --- */

/* Remove newline final de string */
void trimNewline(char *s) {
    size_t L = strlen(s);
    if (L > 0 && s[L-1] == '\n') s[L-1] = '\0';
}

/* Lê string com fgets e remove newline */
void lerString(char *dest, int tamanho) {
    if (fgets(dest, tamanho, stdin) == NULL) {
        dest[0] = '\0';
        return;
    }
    trimNewline(dest);
}

/* Mostra cabeçalho com layout igual ao da imagem */
void mostrarHeader(int qtd, int limite, int sortedBy) {
    printf("=====================================================\n");
    printf("  PLANO DE FUGA - CODIGO DA ILHA (NIVEL MESTRE)\n");
    printf("=====================================================\n");
    printf("Itens na Mochila: %d/%d\n", qtd, limite);
    printf("Status da Ordenacao por Nome: %s\n\n", (sortedBy == 1) ? "ORDENADO" : "NAO ORDENADO");
}

/* Mostra vetor formatado */
void mostrarComponentes(Componente comps[], int n) {
    printf("\n-- Inventario (%d itens) -----------------------------\n", n);
    printf("%-3s | %-28s | %-15s | %s\n", "No", "Nome", "Tipo", "Prioridade");
    printf("----+------------------------------+-----------------+-----------\n");
    for (int i = 0; i < n; i++) {
        printf("%-3d | %-28s | %-15s | %-9d\n", i+1, comps[i].nome, comps[i].tipo, comps[i].prioridade);
    }
    printf("-----------------------------------------------------\n\n");
}

/* Comparação case-insensitive: retorna <0, 0, >0 como strcmp */
int strcmpIgnoreCase(const char *a, const char *b) {
    while (*a && *b) {
        char ca = tolower((unsigned char)*a);
        char cb = tolower((unsigned char)*b);
        if (ca != cb) return (unsigned char)ca - (unsigned char)cb;
        a++; b++;
    }
    return (unsigned char)tolower((unsigned char)*a) - (unsigned char)tolower((unsigned char)*b);
}

/* Copia vetor de componentes */
void copiarComponentes(Componente dest[], Componente src[], int n) {
    for (int i = 0; i < n; i++) dest[i] = src[i];
}

/* Bubble sort por nome (case-insensitive). Conta comparações (cada strcmpIgnoreCase é 1). */
long long bubbleSortNome(Componente v[], int n) {
    long long comparacoes = 0;
    int trocou;
    for (int i = 0; i < n - 1; i++) {
        trocou = 0;
        for (int j = 0; j < n - 1 - i; j++) {
            comparacoes++;
            if (strcmpIgnoreCase(v[j].nome, v[j+1].nome) > 0) {
                Componente tmp = v[j];
                v[j] = v[j+1];
                v[j+1] = tmp;
                trocou = 1;
            }
        }
        if (!trocou) break;
    }
    return comparacoes;
}

/* Insertion sort por tipo (case-insensitive). Conta comparações */
long long insertionSortTipo(Componente v[], int n) {
    long long comparacoes = 0;
    for (int i = 1; i < n; i++) {
        Componente chave = v[i];
        int j = i - 1;
        while (j >= 0) {
            comparacoes++;
            if (strcmpIgnoreCase(v[j].tipo, chave.tipo) > 0) {
                v[j+1] = v[j];
                j--;
            } else break;
        }
        v[j+1] = chave;
    }
    return comparacoes;
}

/* Selection sort por prioridade (DECRESCENTE: maior prioridade primeiro). Conta comparações */
long long selectionSortPrioridade(Componente v[], int n) {
    long long comparacoes = 0;
    for (int i = 0; i < n - 1; i++) {
        int idxMax = i;
        for (int j = i + 1; j < n; j++) {
            comparacoes++;
            if (v[j].prioridade > v[idxMax].prioridade) {
                idxMax = j;
            }
        }
        if (idxMax != i) {
            Componente tmp = v[i];
            v[i] = v[idxMax];
            v[idxMax] = tmp;
        }
    }
    return comparacoes;
}

/* Busca binária por nome (vetor deve estar ordenado por nome - case-insensitive). 
   Devolve índice encontrado ou -1. contabiliza comparações via ponteiro. */
int buscaBinariaPorNome(Componente v[], int n, const char chave[], long long *comparacoes) {
    int l = 0, r = n - 1;
    *comparacoes = 0;
    while (l <= r) {
        int m = l + (r - l) / 2;
        (*comparacoes)++;
        int cmp = strcmpIgnoreCase(v[m].nome, chave);
        if (cmp == 0) return m;
        else if (cmp < 0) l = m + 1;
        else r = m - 1;
    }
    return -1;
}

/* medirTempo: executa algoritmo em cópia e retorna tempo em segundos; devolve comparações também */
double medirTempo(long long (*algoritmo)(Componente[], int), Componente arr[], int n, long long *comparacoes) {
    Componente tmp[MAX_ITENS];
    copiarComponentes(tmp, arr, n);
    clock_t start = clock();
    *comparacoes = algoritmo(tmp, n);
    clock_t end = clock();
    return ((double)(end - start)) / CLOCKS_PER_SEC;
}

/* --- Função principal com menu e painel secundário --- */
int main() {
    Componente comps[MAX_ITENS];
    int n = 0;
    int opcao = -1;
    char buffer[128];
    int sortedBy = 0; // 0=nenhum, 1=nome, 2=tipo, 3=prioridade

    while (1) {
        mostrarHeader(n, MAX_ITENS, sortedBy);

        printf("1. Adicionar Componente\n");
        printf("2. Descartar Componente\n");
        printf("3. Listar Componentes (Inventario)\n");
        printf("4. Organizar Mochila (Ordenar Componentes)\n");
        printf("5. Busca Binaria por Componente-Chave (por nome)\n");
        printf("0. ATIVAR TORRE DE FUGA (Sair)\n");
        printf("-----------------------------------------------------\n");
        printf("Escolha uma opcao: ");

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("\nEntrada encerrada. Saindo...\n");
            break;
        }
        opcao = atoi(buffer);

        if (opcao == 0) {
            printf("\nATIVANDO TORRE DE FUGA... Saindo do sistema.\n");
            break;
        }

        switch (opcao) {
            case 1: { // Adicionar componente
                if (n >= MAX_ITENS) {
                    printf("\nMochila cheia! (limite %d itens)\n\n", MAX_ITENS);
                    break;
                }
                printf("\n--- Cadastro do Componente (%d) ---\n", n+1);
                printf("Nome: ");
                lerString(comps[n].nome, MAX_NOME);
                if (strlen(comps[n].nome) == 0) {
                    printf("Nome invalido. Operacao cancelada.\n\n");
                    break;
                }
                printf("Tipo: ");
                lerString(comps[n].tipo, MAX_TIPO);
                int pri = 0;
                do {
                    printf("Prioridade (1 a 10): ");
                    if (fgets(buffer, sizeof(buffer), stdin) == NULL) { pri = 1; break; }
                    pri = atoi(buffer);
                    if (pri < 1 || pri > 10) printf("Prioridade invalida. Tente novamente.\n");
                    else break;
                } while (1);
                comps[n].prioridade = pri;
                n++;
                sortedBy = 0; // invalida ordenacao anterior
                printf("Componente cadastrado com sucesso!\n\n");
                break;
            }

            case 2: { // Descartar componente (2ª posição do menu)
                if (n == 0) {
                    printf("\nNenhum componente cadastrado para descartar.\n\n");
                    break;
                }
                mostrarComponentes(comps, n);
                printf("Digite o numero do componente que deseja descartar: ");
                if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                    printf("Entrada invalida.\n\n");
                    break;
                }
                int idx = atoi(buffer) - 1;
                if (idx < 0 || idx >= n) {
                    printf("Indice invalido.\n\n");
                    break;
                }
                // desloca elementos para cima
                for (int i = idx; i < n - 1; i++) comps[i] = comps[i+1];
                n--;
                sortedBy = 0;
                printf("Componente descartado com sucesso!\n\n");
                break;
            }

            case 3: { // Listar componentes
                if (n == 0) {
                    printf("\nNenhum componente cadastrado.\n\n");
                    break;
                }
                mostrarComponentes(comps, n);
                break;
            }

            case 4: { // Painel secundário de ordenação
                if (n == 0) {
                    printf("\nNenhum componente cadastrado para ordenar.\n\n");
                    break;
                }
                int opcOrd = -1;
                do {
                    printf("\n===== PAINEL DE ORDENACAO =====\n");
                    printf("1 - Ordenar por NOME (Bubble Sort)\n");
                    printf("2 - Ordenar por TIPO (Insertion Sort)\n");
                    printf("3 - Ordenar por PRIORIDADE (Selection Sort)\n");
                    printf("0 - Voltar ao menu principal\n");
                    printf("Escolha uma opcao: ");
                    if (fgets(buffer, sizeof(buffer), stdin) == NULL) { opcOrd = 0; }
                    else opcOrd = atoi(buffer);

                    if (opcOrd == 0) {
                        printf("Retornando ao menu principal...\n\n");
                        break;
                    } else if (opcOrd == 1) {
                        long long compsCount;
                        double tempo = medirTempo(bubbleSortNome, comps, n, &compsCount);
                        long long realComp = bubbleSortNome(comps, n); // aplica ao vetor real
                        sortedBy = 1;
                        printf("\nOrdenacao por NOME concluida (Bubble Sort).\n");
                        printf("Comparacoes (medicao): %lld\n", compsCount);
                        printf("Comparacoes (aplicadas): %lld\n", realComp);
                        printf("Tempo medido: %.6f segundos\n", tempo);
                        mostrarComponentes(comps, n);
                    } else if (opcOrd == 2) {
                        long long compsCount;
                        double tempo = medirTempo(insertionSortTipo, comps, n, &compsCount);
                        long long realComp = insertionSortTipo(comps, n);
                        sortedBy = 2;
                        printf("\nOrdenacao por TIPO concluida (Insertion Sort).\n");
                        printf("Comparacoes (medicao): %lld\n", compsCount);
                        printf("Comparacoes (aplicadas): %lld\n", realComp);
                        printf("Tempo medido: %.6f segundos\n", tempo);
                        mostrarComponentes(comps, n);
                    } else if (opcOrd == 3) {
                        long long compsCount;
                        double tempo = medirTempo(selectionSortPrioridade, comps, n, &compsCount);
                        long long realComp = selectionSortPrioridade(comps, n);
                        sortedBy = 3;
                        printf("\nOrdenacao por PRIORIDADE concluida (Selection Sort, desc).\n");
                        printf("Comparacoes (medicao): %lld\n", compsCount);
                        printf("Comparacoes (aplicadas): %lld\n", realComp);
                        printf("Tempo medido: %.6f segundos\n", tempo);
                        mostrarComponentes(comps, n);
                    } else {
                        printf("Opcao invalida no painel de ordenacao.\n");
                    }

                } while (opcOrd != 0);
                break;
            }

            case 5: { // Busca binária por nome (apenas se ordenado por nome)
                if (n == 0) {
                    printf("\nNenhum componente cadastrado.\n\n");
                    break;
                }
                if (sortedBy != 1) {
                    printf("\nBusca binaria so pode ser executada apos ordenar por NOME (use a opcao 4 -> 1).\n\n");
                    break;
                }
                char chave[MAX_NOME];
                printf("\nDigite o NOME do componente a buscar (busca binaria, case-insensitive): ");
                lerString(chave, MAX_NOME);
                if (strlen(chave) == 0) {
                    printf("Nome invalido.\n\n");
                    break;
                }
                long long compsBin;
                int idx = buscaBinariaPorNome(comps, n, chave, &compsBin);
                printf("Comparacoes na busca binaria: %lld\n", compsBin);
                if (idx >= 0) {
                    printf("Componente encontrado na posicao %d:\n", idx+1);
                    printf("Nome: %s | Tipo: %s | Prioridade: %d\n\n", comps[idx].nome, comps[idx].tipo, comps[idx].prioridade);
                } else {
                    printf("Componente NAO encontrado.\n\n");
                }
                break;
            }

            default:
                printf("\nOpcao invalida. Tente novamente.\n\n");
                break;
        } // switch

    } // while

    return 0;
}
