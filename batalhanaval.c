#include <stdio.h>
#include <stdlib.h>

#define TAM 10
#define AGUA '~'
#define NAVIO 'N'
#define TIRO_ACERTO 'X'
#define TIRO_ERRO 'o'

typedef struct {
    char grade[TAM][TAM];
} Tabuleiro;

// -------- Protótipos das funções --------
void inicializar_tabuleiro(Tabuleiro *t);
void mostrar_tabuleiro(Tabuleiro *t, int mostrarNavios);
int validar_posicao(Tabuleiro *t, int linha, int coluna, int tamanho, char orientacao);
void colocar_navio(Tabuleiro *t, int linha, int coluna, int tamanho, char orientacao);
void posicionar_frota(Tabuleiro *t);
int atacar(Tabuleiro *t, int linha, int coluna);
int verificar_vitoria(Tabuleiro *t);
void jogar_turno(Tabuleiro *atacante, Tabuleiro *defensor, char *nomeAtacante);
void jogar_partida(Tabuleiro *j1, Tabuleiro *j2);

// -------- Funções básicas --------
void inicializar_tabuleiro(Tabuleiro *t) {
    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {
            t->grade[i][j] = AGUA;
        }
    }
}

void mostrar_tabuleiro(Tabuleiro *t, int mostrarNavios) {
    printf("   ");
    for (int i = 0; i < TAM; i++) {
        printf("%2d", i);
    }
    printf("\n");

    for (int i = 0; i < TAM; i++) {
        printf("%2d ", i);
        for (int j = 0; j < TAM; j++) {
            char c = t->grade[i][j];
            if (!mostrarNavios && c == NAVIO) {
                printf(" ~");
            } else {
                printf(" %c", c);
            }
        }
        printf("\n");
    }
}

// -------- Funções de posicionamento --------
int validar_posicao(Tabuleiro *t, int linha, int coluna, int tamanho, char orientacao) {
    if (orientacao == 'H') {
        if (coluna + tamanho > TAM) return 0;
        for (int j = 0; j < tamanho; j++) {
            if (t->grade[linha][coluna + j] != AGUA) return 0;
        }
    } else if (orientacao == 'V') {
        if (linha + tamanho > TAM) return 0;
        for (int i = 0; i < tamanho; i++) {
            if (t->grade[linha + i][coluna] != AGUA) return 0;
        }
    } else {
        return 0;
    }
    return 1;
}

void colocar_navio(Tabuleiro *t, int linha, int coluna, int tamanho, char orientacao) {
    if (orientacao == 'H') {
        for (int j = 0; j < tamanho; j++) {
            t->grade[linha][coluna + j] = NAVIO;
        }
    } else {
        for (int i = 0; i < tamanho; i++) {
            t->grade[linha + i][coluna] = NAVIO;
        }
    }
}

void posicionar_frota(Tabuleiro *t) {
    int linha, coluna;
    char orientacao;

    int navios_tamanho[] = {5, 4, 3, 3, 2};
    char *navios_nome[] = {"Porta-avioes (5)", "Couracado (4)", "Cruzador (3)", "Submarino (3)", "Destroier (2)"};

    for (int n = 0; n < 5; n++) {
        int valido = 0;
        while (!valido) {
            mostrar_tabuleiro(t, 1);
            printf("\nPosicione o %s\n", navios_nome[n]);
            printf("Digite linha e coluna iniciais: ");
            scanf("%d %d", &linha, &coluna);
            printf("Digite orientacao (H para horizontal, V para vertical): ");
            scanf(" %c", &orientacao);

            if (validar_posicao(t, linha, coluna, navios_tamanho[n], orientacao)) {
                colocar_navio(t, linha, coluna, navios_tamanho[n], orientacao);
                valido = 1;
            } else {
                printf("Posicao invalida! Tente novamente.\n");
            }
        }
    }
}

// -------- Funções de ataque --------
int atacar(Tabuleiro *t, int linha, int coluna) {
    if (t->grade[linha][coluna] == NAVIO) {
        t->grade[linha][coluna] = TIRO_ACERTO;
        printf("Acertou um navio!\n");
        return 1;
    } else if (t->grade[linha][coluna] == AGUA) {
        t->grade[linha][coluna] = TIRO_ERRO;
        printf("Agua!\n");
        return 0;
    } else {
        printf("Ja atirou aqui! Perdeu a vez.\n");
        return -1;
    }
}

int verificar_vitoria(Tabuleiro *t) {
    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {
            if (t->grade[i][j] == NAVIO) {
                return 0;
            }
        }
    }
    return 1;
}

void jogar_turno(Tabuleiro *atacante, Tabuleiro *defensor, char *nomeAtacante) {
    int linha, coluna;
    int resultado = -1;

    printf("\n Vez de %s\n", nomeAtacante);
    mostrar_tabuleiro(defensor, 0);

    while (resultado == -1) {
        printf("Digite linha e coluna para atacar: ");
        scanf("%d %d", &linha, &coluna);
        resultado = atacar(defensor, linha, coluna);
    }
}

void jogar_partida(Tabuleiro *j1, Tabuleiro *j2) {
    int venceu = 0;
    while (!venceu) {
        jogar_turno(j1, j2, "Jogador 1");
        if (verificar_vitoria(j2)) {
            printf("\n Jogador 1 venceu!\n");
            venceu = 1;
            break;
        }

        jogar_turno(j2, j1, "Jogador 2");
        if (verificar_vitoria(j1)) {
            printf("\n Jogador 2 venceu!\n");
            venceu = 1;
            break;
        }
    }
}

// -------- Main --------
int main() {
    Tabuleiro jogador1, jogador2;

    inicializar_tabuleiro(&jogador1);
    inicializar_tabuleiro(&jogador2);

    printf("Jogador 1, posicione sua frota:\n");
    posicionar_frota(&jogador1);
    system("clear || cls");

    printf(" Jogador 2, posicione sua frota:\n");
    posicionar_frota(&jogador2);
    system("clear || cls");

    jogar_partida(&jogador1, &jogador2);

    return 0;
}
