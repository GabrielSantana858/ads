#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_COMPONENTES 20
#define TAM_NOME 30
#define TAM_TIPO 20
#define CHAVE_BUSCA "Chave Mestra"

typedef struct {
    char nome[TAM_NOME];
    char tipo[TAM_TIPO];
    int prioridade;
} Componente;

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void removerNovaLinha(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

void lerString(char *destino, int tamanho) {
    if (fgets(destino, tamanho, stdin) != NULL) {
        removerNovaLinha(destino);
    } else {
        destino[0] = '\0';
    }
}

void cadastrarComponentes(Componente componentes[], int *total) {
    printf("Quantos componentes deseja cadastrar (max %d)? ", MAX_COMPONENTES);
    if (scanf("%d", total) != 1 || *total <= 0 || *total > MAX_COMPONENTES) {
        printf("Valor invalido. Cadastrando 5 componentes padrao.\n");
        *total = 5;
        limparBuffer();
    } else {
        limparBuffer();
    }

    if (*total == 5) {
        Componente padrao[] = {
            {"Chip Central", "Controle", 10},
            {"Suporte Principal", "Suporte", 8},
            {"Propulsor Ionico", "Propulsao", 9},
            {"Cabo de Dados", "Conexao", 5},
            {"Chave Mestra", "Controle", 1}
        };
        for (int i = 0; i < *total; i++) {
            componentes[i] = padrao[i];
        }
        return;
    }
    
    for (int i = 0; i < *total; i++) {
        printf("\n--- Componente %d ---\n", i + 1);
        printf("Nome: ");
        lerString(componentes[i].nome, TAM_NOME);

        printf("Tipo (Controle, Suporte, Propulsao, ...): ");
        lerString(componentes[i].tipo, TAM_TIPO);

        printf("Prioridade (1 a 10): ");
        if (scanf("%d", &componentes[i].prioridade) != 1 || componentes[i].prioridade < 1 || componentes[i].prioridade > 10) {
            printf("Prioridade invalida. Definindo como 5.\n");
            componentes[i].prioridade = 5;
        }
        limparBuffer();
    }
}

void mostrarComponentes(Componente componentes[], int tamanho) {
    printf("\n--- Lista de Componentes ---\n");
    printf("| %-25s | %-15s | %-10s |\n", "Nome", "Tipo", "Prioridade");
    printf("|---------------------------|-----------------|------------|\n");
    for (int i = 0; i < tamanho; i++) {
        printf("| %-25s | %-15s | %-10d |\n", componentes[i].nome, componentes[i].tipo, componentes[i].prioridade);
    }
    printf("-----------------------------------------------------------\n");
}

void bubbleSortNome(Componente componentes[], int tamanho, long long *comparacoes) {
    *comparacoes = 0;
    Componente temp;
    for (int i = 0; i < tamanho - 1; i++) {
        for (int j = 0; j < tamanho - 1 - i; j++) {
            (*comparacoes)++; 
            if (strcmp(componentes[j].nome, componentes[j + 1].nome) > 0) {
                temp = componentes[j];
                componentes[j] = componentes[j + 1];
                componentes[j + 1] = temp;
            }
        }
    }
}

void insertionSortTipo(Componente componentes[], int tamanho, long long *comparacoes) {
    *comparacoes = 0;
    for (int i = 1; i < tamanho; i++) {
        Componente chave = componentes[i];
        int j = i - 1;

        while (j >= 0) {
            (*comparacoes)++; 
            if (strcmp(componentes[j].tipo, chave.tipo) > 0) {
                componentes[j + 1] = componentes[j];
                j = j - 1;
            } else {
                break; 
            }
        }
        componentes[j + 1] = chave;
    }
}

void selectionSortPrioridade(Componente componentes[], int tamanho, long long *comparacoes) {
    *comparacoes = 0;
    for (int i = 0; i < tamanho - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < tamanho; j++) {
            (*comparacoes)++; 
            if (componentes[j].prioridade < componentes[min_idx].prioridade) {
                min_idx = j;
            }
        }
        Componente temp = componentes[min_idx];
        componentes[min_idx] = componentes[i];
        componentes[i] = temp;
    }
}

int buscaBinariaPorNome(Componente componentes[], int tamanho, const char *chave, long long *comparacoes) {
    *comparacoes = 0;
    int esquerda = 0;
    int direita = tamanho - 1;

    while (esquerda <= direita) {
        int meio = esquerda + (direita - esquerda) / 2;
        int resultado = strcmp(componentes[meio].nome, chave);
        
        (*comparacoes)++;

        if (resultado == 0) {
            return meio; 
        } else if (resultado < 0) {
            esquerda = meio + 1; 
        } else {
            direita = meio - 1; 
        }
    }
    return -1; 
}

void medirTempoEExecutar(void (*algoritmo)(Componente[], int, long long*), Componente componentes[], int tamanho, const char *nomeAlgoritmo, long long *comparacoes) {
    Componente copia[MAX_COMPONENTES];
    memcpy(copia, componentes, tamanho * sizeof(Componente));

    clock_t inicio = clock();
    algoritmo(copia, tamanho, comparacoes);
    clock_t fim = clock();

    double tempo_execucao = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

    printf("\n--- Desempenho do %s ---\n", nomeAlgoritmo);
    printf("Numero de comparacoes: %lld\n", *comparacoes);
    printf("Tempo de execucao: %f segundos\n", tempo_execucao);

    memcpy(componentes, copia, tamanho * sizeof(Componente));
}

void menuPrincipal(Componente componentes[], int total) {
    int opcao;
    long long comparacoes = 0;
    int ordenadoPorNome = 0;

    do {
        printf("\n=== Modulo de Organizacao de Componentes ===\n");
        printf("1. Cadastrar Componentes\n");
        printf("2. Bubble Sort (Ordenar por Nome)\n");
        printf("3. Insertion Sort (Ordenar por Tipo)\n");
        printf("4. Selection Sort (Ordenar por Prioridade)\n");
        printf("5. Busca Binaria por Nome (%s)\n", CHAVE_BUSCA);
        printf("6. Mostrar Componentes Atuais\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");

        if (scanf("%d", &opcao) != 1) {
            limparBuffer();
            opcao = -1;
        } else {
            limparBuffer();
        }

        switch (opcao) {
            case 1:
                cadastrarComponentes(componentes, &total);
                ordenadoPorNome = 0;
                break;

            case 2:
                medirTempoEExecutar(bubbleSortNome, componentes, total, "Bubble Sort (Nome)", &comparacoes);
                mostrarComponentes(componentes, total);
                ordenadoPorNome = 1;
                break;

            case 3:
                medirTempoEExecutar(insertionSortTipo, componentes, total, "Insertion Sort (Tipo)", &comparacoes);
                mostrarComponentes(componentes, total);
                ordenadoPorNome = 0;
                break;

            case 4:
                medirTempoEExecutar(selectionSortPrioridade, componentes, total, "Selection Sort (Prioridade)", &comparacoes);
                mostrarComponentes(componentes, total);
                ordenadoPorNome = 0;
                break;

            case 5:
                if (!ordenadoPorNome) {
                    printf("\nERRO: A busca binaria so pode ser aplicada apos a ordenacao por Nome (Opcao 2).\n");
                } else {
                    clock_t inicio = clock();
                    int indice = buscaBinariaPorNome(componentes, total, CHAVE_BUSCA, &comparacoes);
                    clock_t fim = clock();
                    double tempo_execucao = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

                    printf("\n--- Desempenho da Busca Binaria ---\n");
                    printf("Numero de comparacoes: %lld\n", comparacoes);
                    printf("Tempo de execucao: %f segundos\n", tempo_execucao);
                    
                    if (indice != -1) {
                        printf("\nComponente-Chave '%s' encontrado com sucesso no indice %d!\n", CHAVE_BUSCA, indice);
                        printf("Montagem da Torre: ATIVACAO LIBERADA.\n");
                    } else {
                        printf("\nComponente-Chave '%s' NAO encontrado.\n", CHAVE_BUSCA);
                        printf("Montagem da Torre: ATIVACAO BLOQUEADA.\n");
                    }
                }
                break;

            case 6:
                mostrarComponentes(componentes, total);
                break;

            case 0:
                printf("\nSaindo do sistema. Boa fuga!\n");
                break;

            default:
                printf("\nOpcao invalida. Tente novamente.\n");
                break;
        }
    } while (opcao != 0);
}

int main() {
    Componente componentes[MAX_COMPONENTES];
    int totalComponentes = 0;

    cadastrarComponentes(componentes, &totalComponentes);
    mostrarComponentes(componentes, totalComponentes);
    
    menuPrincipal(componentes, totalComponentes);

    return 0;
}
