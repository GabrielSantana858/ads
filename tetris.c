#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CAPACIDADE_FILA 5
#define CAPACIDADE_PILHA 3
#define TIPOS_PECA "IOTL"

typedef struct {
    char nome;
    int id;
} Peca;

typedef struct {
    Peca itens[CAPACIDADE_FILA];
    int frente; 
    int tras;   
    int contador; 
} FilaCircular;

typedef struct {
    Peca itens[CAPACIDADE_PILHA];
    int topo; 
} Pilha;

int proximoID = 0;

void inicializarFila(FilaCircular *f);
void inicializarPilha(Pilha *p);
Peca gerarPeca();
void exibirEstado(const FilaCircular *f, const Pilha *p);
void exibirMenu();
void limparBuffer();

int estaVaziaFila(const FilaCircular *f);
int estaCheiaFila(const FilaCircular *f);
void enfileirar(FilaCircular *f, Peca p);
Peca desenfileirar(FilaCircular *f);
Peca pecaFrenteFila(const FilaCircular *f);

int estaVaziaPilha(const Pilha *p);
int estaCheiaPilha(const Pilha *p);
void empilhar(Pilha *p, Peca peca);
Peca desempilhar(Pilha *p);
Peca pecaTopoPilha(const Pilha *p);

void jogarPeca(FilaCircular *f);
void reservarPeca(FilaCircular *f, Pilha *p);
void usarPecaReservada(Pilha *p);
void trocarPecaAtual(FilaCircular *f, Pilha *p);
void trocaMultipla(FilaCircular *f, Pilha *p);

void inicializarFila(FilaCircular *f) {
    f->frente = 0;
    f->tras = -1; 
    f->contador = 0;
}

void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

Peca gerarPeca() {
    Peca novaPeca;
    int indiceTipo = rand() % strlen(TIPOS_PECA);
    novaPeca.nome = TIPOS_PECA[indiceTipo];
    novaPeca.id = proximoID++;
    return novaPeca;
}

void exibirEstado(const FilaCircular *f, const Pilha *p) {
    printf("\n--- Estado Atual ---\n");

    printf("Fila de pecas (%d/%d): ", f->contador, CAPACIDADE_FILA);
    if (estaVaziaFila(f)) {
        printf("[Vazia]\n");
    } else {
        printf("[");
        int i = f->frente;
        for (int count = 0; count < f->contador; count++) {
            printf("%c %d", f->itens[i].nome, f->itens[i].id);
            if (count < f->contador - 1) {
                printf("] [");
            }
            i = (i + 1) % CAPACIDADE_FILA;
        }
        printf("]\n");
    }

    printf("Pilha de reserva (Topo -> base) (%d/%d): ", p->topo + 1, CAPACIDADE_PILHA);
    if (estaVaziaPilha(p)) {
        printf("[Vazia]\n");
    } else {
        printf("[");
        for (int i = p->topo; i >= 0; i--) {
            printf("%c %d", p->itens[i].nome, p->itens[i].id);
            if (i > 0) {
                printf("] [");
            }
        }
        printf("]\n");
    }
    printf("--------------------\n");
}

void exibirMenu() {
    printf("\nOpcoes disponiveis:\n");
    printf("Codigo | Acao\n");
    printf("-------|---------------------------------------------------\n");
    printf("1      | Jogar peca da frente da fila\n");
    printf("2      | Enviar peca da fila para a pilha de reserva\n");
    printf("3      | Usar peca da pilha de reserva\n");
    printf("4      | Trocar peca da frente da fila com o topo da pilha\n");
    printf("5      | Trocar os %d primeiros da fila com as %d pecas da pilha\n", CAPACIDADE_PILHA, CAPACIDADE_PILHA);
    printf("0      | Sair\n");
    printf("Escolha uma opcao: ");
}

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int estaVaziaFila(const FilaCircular *f) {
    return f->contador == 0;
}

int estaCheiaFila(const FilaCircular *f) {
    return f->contador == CAPACIDADE_FILA;
}

void enfileirar(FilaCircular *f, Peca p) {
    if (estaCheiaFila(f)) {
        return; 
    }
    f->tras = (f->tras + 1) % CAPACIDADE_FILA;
    f->itens[f->tras] = p;
    f->contador++;
}

Peca desenfileirar(FilaCircular *f) {
    if (estaVaziaFila(f)) {
        Peca pecaVazia = {'\0', -1}; 
        return pecaVazia;
    }
    Peca peca = f->itens[f->frente];
    f->frente = (f->frente + 1) % CAPACIDADE_FILA;
    f->contador--;
    return peca;
}

Peca pecaFrenteFila(const FilaCircular *f) {
    if (estaVaziaFila(f)) {
        Peca pecaVazia = {'\0', -1}; 
        return pecaVazia;
    }
    return f->itens[f->frente];
}

int estaVaziaPilha(const Pilha *p) {
    return p->topo == -1;
}

int estaCheiaPilha(const Pilha *p) {
    return p->topo == CAPACIDADE_PILHA - 1;
}

void empilhar(Pilha *p, Peca peca) {
    if (estaCheiaPilha(p)) {
        return;
    }
    p->topo++;
    p->itens[p->topo] = peca;
}

Peca desempilhar(Pilha *p) {
    if (estaVaziaPilha(p)) {
        Peca pecaVazia = {'\0', -1}; 
        return pecaVazia;
    }
    return p->itens[p->topo--];
}

Peca pecaTopoPilha(const Pilha *p) {
    if (estaVaziaPilha(p)) {
        Peca pecaVazia = {'\0', -1}; 
        return pecaVazia;
    }
    return p->itens[p->topo];
}

void jogarPeca(FilaCircular *f) {
    if (estaVaziaFila(f)) {
        printf("Acao: Fila esta vazia. Nenhuma peca para jogar.\n");
        return;
    }

    Peca jogada = desenfileirar(f);
    printf("Acao: Peca [%c %d] jogada (removida da fila).\n", jogada.nome, jogada.id);

    if (!estaCheiaFila(f)) {
        Peca novaPeca = gerarPeca();
        enfileirar(f, novaPeca);
        printf("Acao: Nova peca [%c %d] gerada e adicionada ao final da fila.\n", novaPeca.nome, novaPeca.id);
    }
}

void reservarPeca(FilaCircular *f, Pilha *p) {
    if (estaVaziaFila(f)) {
        printf("Acao: Fila esta vazia. Nenhuma peca para reservar.\n");
        return;
    }
    if (estaCheiaPilha(p)) {
        printf("Acao: Pilha de reserva esta cheia. Nao e possivel reservar mais pecas.\n");
        return;
    }

    Peca reservada = desenfileirar(f);
    empilhar(p, reservada);
    printf("Acao: Peca [%c %d] movida da frente da fila para o topo da pilha de reserva.\n", reservada.nome, reservada.id);

    if (!estaCheiaFila(f)) {
        Peca novaPeca = gerarPeca();
        enfileirar(f, novaPeca);
        printf("Acao: Nova peca [%c %d] gerada e adicionada ao final da fila.\n", novaPeca.nome, novaPeca.id);
    }
}

void usarPecaReservada(Pilha *p) {
    if (estaVaziaPilha(p)) {
        printf("Acao: Pilha de reserva esta vazia. Nenhuma peca para usar.\n");
        return;
    }

    Peca usada = desempilhar(p);
    printf("Acao: Peca reservada [%c %d] usada (removida do topo da pilha).\n", usada.nome, usada.id);
}

void trocarPecaAtual(FilaCircular *f, Pilha *p) {
    if (estaVaziaFila(f) || estaVaziaPilha(p)) {
        printf("Acao: Troca invalida. Fila e/ou Pilha estao vazias.\n");
        return;
    }

    Peca pecaFila = pecaFrenteFila(f); 
    Peca pecaPilha = pecaTopoPilha(p);

    f->itens[f->frente] = pecaPilha;
    
    p->itens[p->topo] = pecaFila;

    printf("Acao: Peca [%c %d] (fila) trocada por [%c %d] (pilha).\n", pecaFila.nome, pecaFila.id, pecaPilha.nome, pecaPilha.id);
}

void trocaMultipla(FilaCircular *f, Pilha *p) {
    if (f->contador < CAPACIDADE_PILHA || p->topo + 1 < CAPACIDADE_PILHA) {
        printf("Acao: Troca multipla invalida. Necessario pelo menos %d pecas na Fila E na Pilha.\n", CAPACIDADE_PILHA);
        return;
    }

    Peca tempFila[CAPACIDADE_PILHA];
    Peca tempPilha[CAPACIDADE_PILHA];

    int idxFila = f->frente;
    for (int i = 0; i < CAPACIDADE_PILHA; i++) {
        tempFila[i] = f->itens[idxFila];
        idxFila = (idxFila + 1) % CAPACIDADE_FILA;
    }

    for (int i = 0; i < CAPACIDADE_PILHA; i++) {
        tempPilha[i] = p->itens[p->topo - i];
    }

    idxFila = f->frente;
    for (int i = 0; i < CAPACIDADE_PILHA; i++) {
        f->itens[idxFila] = tempPilha[i];
        idxFila = (idxFila + 1) % CAPACIDADE_FILA;
    }

    for (int i = 0; i < CAPACIDADE_PILHA; i++) {
        p->itens[p->topo - i] = tempFila[i];
    }
    
    printf("Acao: Troca realizada entre os %d primeiros da fila e as %d pecas da pilha.\n", CAPACIDADE_PILHA, CAPACIDADE_PILHA);
}

int main() {
    srand(time(NULL));

    FilaCircular fila;
    Pilha pilha;
    int opcao;

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    for (int i = 0; i < CAPACIDADE_FILA; i++) {
        enfileirar(&fila, gerarPeca());
    }

    printf("--- Gerenciador de Pecas Estrategicas ---\n");
    
    do {
        exibirEstado(&fila, &pilha);
        exibirMenu();

        if (scanf("%d", &opcao) != 1) {
            limparBuffer();
            opcao = -1; 
        } else {
            limparBuffer();
        }

        switch (opcao) {
            case 1:
                jogarPeca(&fila);
                break;
            case 2:
                reservarPeca(&fila, &pilha);
                break;
            case 3:
                usarPecaReservada(&pilha);
                break;
            case 4:
                trocarPecaAtual(&fila, &pilha);
                break;
            case 5:
                trocaMultipla(&fila, &pilha);
                break;
            case 0:
                printf("Encerrando o Gerenciador de Pecas. Adeus!\n");
                break;
            default:
                printf("Opcao invalida. Por favor, tente novamente.\n");
                break;
        }
    } while (opcao != 0);

    return 0;
}
