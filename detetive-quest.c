#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_CHAVE 50
#define MAX_VALOR 30
#define TAMANHO_HASH 10
#define PISTAS_MINIMAS 2

typedef struct NoComodo {
    char nome[MAX_VALOR];
    struct NoComodo *esquerda;
    struct NoComodo *direita;
} NoComodo;

typedef struct NoPista {
    char pista[MAX_CHAVE];
    struct NoPista *esquerda;
    struct NoPista *direita;
} NoPista;

typedef struct ItemHash {
    char pista[MAX_CHAVE];
    char suspeito[MAX_VALOR];
    struct ItemHash *proximo;
} ItemHash;

typedef struct {
    ItemHash *tabela[TAMANHO_HASH];
} TabelaHash;

// --- Funcoes Auxiliares ---

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// --- Funcoes do Mapa (Árvore Binária) ---

NoComodo *criarSala(const char *nome) {
    NoComodo *novo = (NoComodo *)malloc(sizeof(NoComodo));
    if (novo == NULL) {
        perror("Erro de alocacao para NoComodo");
        exit(EXIT_FAILURE);
    }
    strcpy(novo->nome, nome);
    novo->esquerda = NULL;
    novo->direita = NULL;
    return novo;
}

char *obterPista(const char *nomeComodo) {
    if (strcmp(nomeComodo, "Entrada") == 0) {
        return "Pegadas de lama frescas"; 
    } else if (strcmp(nomeComodo, "Salao") == 0) {
        return "Lenço bordado com 'A'"; 
    } else if (strcmp(nomeComodo, "Biblioteca") == 0) {
        return "Cigarro cubano apagado"; 
    } else if (strcmp(nomeComodo, "Cozinha") == 0) {
        return "Faca de prata faltando";
    } else if (strcmp(nomeComodo, "QuartoPrincipal") == 0) {
        return "Diario de bordo revirado";
    } else if (strcmp(nomeComodo, "JardimInverno") == 0) {
        return "Restos de vidro quebrado";
    } else {
        return NULL;
    }
}

// --- Funcoes da BST de Pistas ---

NoPista *inserirPista(NoPista *raiz, const char *pista) {
    if (raiz == NULL) {
        NoPista *novo = (NoPista *)malloc(sizeof(NoPista));
        if (novo == NULL) {
            perror("Erro de alocacao para NoPista");
            exit(EXIT_FAILURE);
        }
        strcpy(novo->pista, pista);
        novo->esquerda = NULL;
        novo->direita = NULL;
        return novo;
    }

    int cmp = strcmp(pista, raiz->pista);
    if (cmp < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    } else if (cmp > 0) {
        raiz->direita = inserirPista(raiz->direita, pista);
    } 

    return raiz;
}

void listarPistas(NoPista *raiz) {
    if (raiz != NULL) {
        listarPistas(raiz->esquerda);
        printf("- %s\n", raiz->pista);
        listarPistas(raiz->direita);
    }
}

// --- Funcoes da Tabela Hash ---

unsigned int hash(const char *chave) {
    unsigned int valor = 0;
    for (int i = 0; chave[i] != '\0'; i++) {
        valor = valor * 31 + chave[i];
    }
    return valor % TAMANHO_HASH;
}

void inicializarHash(TabelaHash *t) {
    for (int i = 0; i < TAMANHO_HASH; i++) {
        t->tabela[i] = NULL;
    }
}

void inserirNaHash(TabelaHash *t, const char *pista, const char *suspeito) {
    unsigned int indice = hash(pista);
    
    ItemHash *novoItem = (ItemHash *)malloc(sizeof(ItemHash));
    if (novoItem == NULL) {
        perror("Erro de alocacao para ItemHash");
        exit(EXIT_FAILURE);
    }
    strcpy(novoItem->pista, pista);
    strcpy(novoItem->suspeito, suspeito);
    novoItem->proximo = t->tabela[indice];
    t->tabela[indice] = novoItem;
}

char *encontrarSuspeito(TabelaHash *t, const char *pista) {
    unsigned int indice = hash(pista);
    ItemHash *atual = t->tabela[indice];

    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0) {
            return atual->suspeito;
        }
        atual = atual->proximo;
    }
    return NULL; 
}

// --- Funcoes de Jogo e Logica ---

void explorarSalas(NoComodo *raiz, NoPista **raizPistas, TabelaHash *hash) {
    NoComodo *atual = raiz;
    char acao[10];

    printf("--- Iniciando a Exploracao da Mansao ---\n");
    
    while (atual != NULL) {
        printf("\nVoce esta em: %s\n", atual->nome);

        char *pistaStr = obterPista(atual->nome);
        if (pistaStr != NULL) {
            printf("PISTA ENCONTRADA: \"%s\"\n", pistaStr);
            *raizPistas = inserirPista(*raizPistas, pistaStr);
            printf("Pista adicionada ao seu caderno de notas.\n");
        } else {
            printf("Nenhuma pista significativa encontrada aqui.\n");
        }

        printf("Para onde ir? [E]squerda / [D]ireita / [S]air: ");
        if (fgets(acao, sizeof(acao), stdin) == NULL) break;
        acao[0] = tolower(acao[0]);

        if (acao[0] == 'e') {
            if (atual->esquerda != NULL) {
                atual = atual->esquerda;
            } else {
                printf("Caminho bloqueado (sem comodo a esquerda).\n");
            }
        } else if (acao[0] == 'd') {
            if (atual->direita != NULL) {
                atual = atual->direita;
            } else {
                printf("Caminho bloqueado (sem comodo a direita).\n");
            }
        } else if (acao[0] == 's') {
            printf("Encerrando a exploracao.\n");
            break;
        } else {
            printf("Acao invalida. Tente novamente.\n");
        }
    }
}

void contarPistasPorSuspeito(NoPista *raizPistas, TabelaHash *hash, const char *suspeitoAlvo, int *contador) {
    if (raizPistas != NULL) {
        contarPistasPorSuspeito(raizPistas->esquerda, hash, suspeitoAlvo, contador);
        
        char *suspeitoPista = encontrarSuspeito(hash, raizPistas->pista);
        if (suspeitoPista != NULL && strcmp(suspeitoPista, suspeitoAlvo) == 0) {
            (*contador)++;
        }

        contarPistasPorSuspeito(raizPistas->direita, hash, suspeitoAlvo, contador);
    }
}

void verificarSuspeitoFinal(NoPista *raizPistas, TabelaHash *hash) {
    char acusacao[MAX_VALOR];
    int pistasConfirmadas = 0;

    printf("\n--- Fase de Julgamento Final ---\n");
    printf("Pistas Coletadas:\n");
    if (raizPistas == NULL) {
        printf("Nenhuma pista foi coletada.\n");
    } else {
        listarPistas(raizPistas);
    }
    printf("--------------------------------\n");

    printf("Quem voce acusa? (Digite o nome do Suspeito): ");
    if (fgets(acusacao, sizeof(acusacao), stdin) == NULL) return;
    size_t len = strlen(acusacao);
    if (len > 0 && acusacao[len - 1] == '\n') {
        acusacao[len - 1] = '\0';
    }

    if (strlen(acusacao) == 0) {
        printf("Acusacao vazia. O caso permanece aberto.\n");
        return;
    }

    contarPistasPorSuspeito(raizPistas, hash, acusacao, &pistasConfirmadas);

    printf("\n-- Resultado da Investigacao --\n");
    printf("Acusacao: %s\n", acusacao);
    printf("Pistas que apontam para %s: %d\n", acusacao, pistasConfirmadas);

    if (pistasConfirmadas >= PISTAS_MINIMAS) {
        printf("\nACUSACAO SUSTENTADA! Pistas suficientes (%d) para provar a culpa de %s.\n", PISTAS_MINIMAS, acusacao);
        printf("VITORIA! O mistério foi resolvido.\n");
    } else {
        printf("\nACUSACAO INSUSTENTAVEL. Apenas %d pistas. Necessario pelo menos %d para acusar %s.\n", pistasConfirmadas, PISTAS_MINIMAS, acusacao);
        printf("FALHA! O verdadeiro culpado escapou.\n");
    }
}

// --- Funcoes de Liberacao de Memoria ---

void liberarComodos(NoComodo *raiz) {
    if (raiz != NULL) {
        liberarComodos(raiz->esquerda);
        liberarComodos(raiz->direita);
        free(raiz);
    }
}

void liberarPistas(NoPista *raiz) {
    if (raiz != NULL) {
        liberarPistas(raiz->esquerda);
        liberarPistas(raiz->direita);
        free(raiz);
    }
}

void liberarHash(TabelaHash *t) {
    for (int i = 0; i < TAMANHO_HASH; i++) {
        ItemHash *atual = t->tabela[i];
        while (atual != NULL) {
            ItemHash *proximo = atual->proximo;
            free(atual);
            atual = proximo;
        }
        t->tabela[i] = NULL;
    }
}

// --- Funcao Principal ---

int main() {
    NoComodo *mansao = NULL;
    NoPista *cadernoPistas = NULL;
    TabelaHash suspeitos;

    // 1. Montagem da Árvore Binária (Mapa Fixo)
    mansao = criarSala("Entrada");
    mansao->esquerda = criarSala("Salao");
    mansao->direita = criarSala("Cozinha");
    mansao->esquerda->esquerda = criarSala("Biblioteca");
    mansao->esquerda->direita = criarSala("QuartoPrincipal");
    mansao->direita->esquerda = criarSala("JardimInverno");

    // 2. Inicializacao e Preenchimento da Tabela Hash (Pistas -> Suspeitos)
    inicializarHash(&suspeitos);
    inserirNaHash(&suspeitos, "Pegadas de lama frescas", "Mordomo");
    inserirNaHash(&suspeitos, "Lenço bordado com 'A'", "Amante");
    inserirNaHash(&suspeitos, "Cigarro cubano apagado", "Cozinheira"); 
    inserirNaHash(&suspeitos, "Faca de prata faltando", "Cozinheira");
    inserirNaHash(&suspeitos, "Diario de bordo revirado", "Amante");
    inserirNaHash(&suspeitos, "Restos de vidro quebrado", "Mordomo");

    // 3. Exploração da Mansão e Coleta de Pistas (BST)
    explorarSalas(mansao, &cadernoPistas, &suspeitos);

    // 4. Verificação Final e Acusação
    verificarSuspeitoFinal(cadernoPistas, &suspeitos);

    // 5. Liberacao de Memória
    liberarComodos(mansao);
    liberarPistas(cadernoPistas);
    liberarHash(&suspeitos);

    return 0;
}
