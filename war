#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_MISSOES 5
#define MAX_TERRITORIOS 5
#define TAM_NOME 30
#define TAM_COR 10
#define TAM_MISSAO 100

typedef struct {
    char nome[TAM_NOME];
    char cor[TAM_COR];
    int tropas;
} Territorio;

void inicializarJogo(Territorio** mapa, char** missaoJogador, int totalTerritorios);
void atribuirMissao(char* destino, char* missoes[], int totalMissoes);
void exibirMissao(char* missao);
void exibirMapa(Territorio* mapa, int tamanho);
int verificarMissao(char* missao, Territorio* mapa, int tamanho);
void atacar(Territorio* atacante, Territorio* defensor);
int temTerritorioInimigo(Territorio* mapa, int tamanho, const char* corAtual);
void liberarMemoria(Territorio* mapa, char* missaoJogador);
void inicializarMapa(Territorio* mapa, int totalTerritorios);
void inicializarMissaoJogador(char** missaoJogador);

void inicializarMapa(Territorio* mapa, int totalTerritorios) {
    strcpy(mapa[0].nome, "A");
    strcpy(mapa[0].cor, "Azul");
    mapa[0].tropas = 3;

    strcpy(mapa[1].nome, "B");
    strcpy(mapa[1].cor, "Vermelho");
    mapa[1].tropas = 2;

    strcpy(mapa[2].nome, "C");
    strcpy(mapa[2].cor, "Azul");
    mapa[2].tropas = 4;

    strcpy(mapa[3].nome, "D");
    strcpy(mapa[3].cor, "Vermelho");
    mapa[3].tropas = 1;

    strcpy(mapa[4].nome, "E");
    strcpy(mapa[4].cor, "Azul");
    mapa[4].tropas = 5;
}

void inicializarMissaoJogador(char** missaoJogador) {
    *missaoJogador = (char*)malloc(TAM_MISSAO * sizeof(char));
    if (*missaoJogador == NULL) {
        printf("Erro na alocacao de memoria para a missao.\n");
        exit(1);
    }
}

void inicializarJogo(Territorio** mapa, char** missaoJogador, int totalTerritorios) {
    *mapa = (Territorio*)calloc(totalTerritorios, sizeof(Territorio));
    if (*mapa == NULL) {
        printf("Erro na alocacao de memoria para o mapa.\n");
        exit(1);
    }

    inicializarMapa(*mapa, totalTerritorios);

    inicializarMissaoJogador(missaoJogador);
}

void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    int indiceSorteado = rand() % totalMissoes;
    strcpy(destino, missoes[indiceSorteado]);
}

void exibirMissao(char* missao) {
    printf("Sua Missao Estrategica: %s\n", missao);
}

void exibirMapa(Territorio* mapa, int tamanho) {
    printf("--- Mapa Atual ---\n");
    for (int i = 0; i < tamanho; i++) {
        printf("Territorio %s: Cor %s, Tropas %d\n", mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("------------------\n");
}

int verificarMissao(char* missao, Territorio* mapa, int tamanho) {
    if (strcmp(missao, "Conquistar 3 territorios azuis.") == 0) {
        int azuis = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "Azul") == 0) {
                azuis++;
            }
        }
        return azuis >= 3;
    } else if (strcmp(missao, "Eliminar todas as tropas da cor vermelha.") == 0) {
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "Vermelho") == 0) {
                return 0;
            }
        }
        return 1;
    } else if (strcmp(missao, "Ter 4 ou mais territorios.") == 0) {
        int territoriosProprios = 0;
        const char* minhaCor = "Azul"; 
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, minhaCor) == 0) {
                territoriosProprios++;
            }
        }
        return territoriosProprios >= 4;
    } else if (strcmp(missao, "Conquistar um territorio com mais de 3 tropas.") == 0) {
        const char* minhaCor = "Azul"; 
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, minhaCor) == 0 && mapa[i].tropas > 3) {
                return 1;
            }
        }
        return 0;
    } else if (strcmp(missao, "Ter mais territorios que o inimigo.") == 0) {
        int territoriosAzuis = 0;
        int territoriosVermelhos = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "Azul") == 0) {
                territoriosAzuis++;
            } else if (strcmp(mapa[i].cor, "Vermelho") == 0) {
                territoriosVermelhos++;
            }
        }
        return territoriosAzuis > territoriosVermelhos;
    }

    return 0;
}

void atacar(Territorio* atacante, Territorio* defensor) {
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("Ataque invalido: Nao pode atacar um territorio amigo.\n");
        return;
    }

    if (atacante->tropas < 2) {
        printf("Ataque invalido: Necessario ao menos 2 tropas para atacar.\n");
        return;
    }

    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;

    printf("Ataque de %s (%s, Tropas %d) contra %s (%s, Tropas %d)\n",
           atacante->nome, atacante->cor, atacante->tropas,
           defensor->nome, defensor->cor, defensor->tropas);
    printf("Dado Atacante: %d, Dado Defensor: %d\n", dadoAtacante, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("Atacante venceu!\n");
        int tropasTransferir = atacante->tropas / 2;

        strcpy(defensor->cor, atacante->cor);
        defensor->tropas = tropasTransferir;
        atacante->tropas -= tropasTransferir;

        printf("Territorio %s conquistado por %s. Tropas transferidas: %d.\n",
               defensor->nome, defensor->cor, tropasTransferir);
    } else {
        printf("Defensor venceu!\n");
        atacante->tropas--;
        printf("Atacante perdeu 1 tropa. Tropas restantes: %d.\n", atacante->tropas);
    }
}

int temTerritorioInimigo(Territorio* mapa, int tamanho, const char* corAtual) {
    for (int i = 0; i < tamanho; i++) {
        if (strcmp(mapa[i].cor, corAtual) != 0) {
            return 1;
        }
    }
    return 0;
}

void liberarMemoria(Territorio* mapa, char* missaoJogador) {
    free(mapa);
    free(missaoJogador);
    printf("Memoria alocada dinamicamente liberada.\n");
}

int main() {
    srand(time(NULL));

    char* missoes[MAX_MISSOES] = {
        "Conquistar 3 territorios azuis.",
        "Eliminar todas as tropas da cor vermelha.",
        "Ter 4 ou mais territorios.",
        "Conquistar um territorio com mais de 3 tropas.",
        "Ter mais territorios que o inimigo."
    };

    Territorio* mapa;
    char* missaoJogador;

    inicializarJogo(&mapa, &missaoJogador, MAX_TERRITORIOS);

    atribuirMissao(missaoJogador, missoes, MAX_MISSOES);

    printf("Bem-vindo ao War Estruturado!\n");
    exibirMissao(missaoJogador);

    int turno = 1;
    int vencedor = 0;

    while (turno <= 3 && !vencedor && temTerritorioInimigo(mapa, MAX_TERRITORIOS, "Azul")) {
        printf("\n--- Turno %d ---\n", turno);
        exibirMapa(mapa, MAX_TERRITORIOS);

        int atacanteIdx, defensorIdx;
        char corAtacante[TAM_COR];
        printf("Jogador Azul (Sua vez):\n");

        printf("Escolha o indice do territorio ATACANTE (0 a %d): ", MAX_TERRITORIOS - 1);
        if (scanf("%d", &atacanteIdx) != 1 || atacanteIdx < 0 || atacanteIdx >= MAX_TERRITORIOS) {
            printf("Entrada invalida. Pulando ataque.\n");
            while (getchar() != '\n');
            turno++;
            continue;
        }

        strcpy(corAtacante, mapa[atacanteIdx].cor);
        if (strcmp(corAtacante, "Azul") != 0) {
            printf("Ataque invalido: Escolha um territorio com sua cor (Azul).\n");
            turno++;
            continue;
        }

        printf("Escolha o indice do territorio DEFENSOR (0 a %d): ", MAX_TERRITORIOS - 1);
        if (scanf("%d", &defensorIdx) != 1 || defensorIdx < 0 || defensorIdx >= MAX_TERRITORIOS) {
            printf("Entrada invalida. Pulando ataque.\n");
            while (getchar() != '\n');
            turno++;
            continue;
        }

        if (atacanteIdx == defensorIdx) {
            printf("Ataque invalido: Atacante e defensor devem ser diferentes.\n");
            turno++;
            continue;
        }

        atacar(&mapa[atacanteIdx], &mapa[defensorIdx]);

        if (verificarMissao(missaoJogador, mapa, MAX_TERRITORIOS)) {
            vencedor = 1;
            break;
        }

        turno++;
    }

    printf("\n--- Fim do Jogo ---\n");
    exibirMapa(mapa, MAX_TERRITORIOS);

    if (vencedor) {
        printf("Parabens! O jogador Azul cumpriu a missao: %s\n", missaoJogador);
        printf("VENCEDOR: Jogador Azul!\n");
    } else if (!temTerritorioInimigo(mapa, MAX_TERRITORIOS, "Azul")) {
        printf("Parabens! O jogador Azul conquistou todos os territorios!\n");
        printf("VENCEDOR: Jogador Azul!\n");
    } else {
        printf("Ninguem cumpriu a missao ou conquistou todos os territorios nos 3 turnos.\n");
        printf("Missao do jogador Azul: %s\n", missaoJogador);
    }

    liberarMemoria(mapa, missaoJogador);

    return 0;
}
