#include <stdio.h>
#include <stdlib.h>

#define TAM 10
#define AGUA '~'
#define NAVIO 'N'
#define TIRO_ACERTO 'X'
#define TIRO_ERRO 'o'

// Estrutura do tabuleiro
typedef struct {
    char grade[TAM][TAM];
} Tabuleiro;

// -------- Protótipos --------
void inicializar_tabuleiro(Tabuleiro *t);
void mostrar_tabuleiro(Tabuleiro *t, int mostrarNavios);
int validar_posicao(Tabuleiro *t, int linha, int coluna, int tamanho, char orientacao);
void colocar_navio(Tabuleiro *t, int linha, int coluna, int tamanho, char orientacao);
void posicionar_frota(Tabuleiro *t);
int atacar(Tabuleiro *t, int linha, int coluna);
int verificar_vitoria(Tabuleiro *t);
int jogar_turno(Tabuleiro *atacante, Tabuleiro *defensor, char *nomeAtacante, char *nomeDefensor);
void jogar_partida(Tabuleiro *j1, Tabuleiro *j2);
void mostrar_vencedor(char *nomeVencedor);
void menu_inicial();
void limpar_tela();

// -------- Funções --------
void inicializar_tabuleiro(Tabuleiro *t) {
    for (int i = 0; i < TAM; i++)
        for (int j = 0; j < TAM; j++)
            t->grade[i][j] = AGUA;
}

void mostrar_tabuleiro(Tabuleiro *t, int mostrarNavios) {
    printf("   ");
    for (int i = 0; i < TAM; i++)
        printf("%2d", i);
    printf("\n");

    for (int i = 0; i < TAM; i++) {
        printf("%2d ", i);
        for (int j = 0; j < TAM; j++) {
            char c = t->grade[i][j];
            if (!mostrarNavios && c == NAVIO)
                printf(" ~");
            else
                printf(" %c", c);
        }
        printf("\n");
    }
}

int validar_posicao(Tabuleiro *t, int linha, int coluna, int tamanho, char orientacao) {
    if (orientacao == 'H') {
        if (coluna + tamanho > TAM) return 0;
        for (int j = 0; j < tamanho; j++)
            if (t->grade[linha][coluna + j] != AGUA) return 0;
    } else if (orientacao == 'V') {
        if (linha + tamanho > TAM) return 0;
        for (int i = 0; i < tamanho; i++)
            if (t->grade[linha + i][coluna] != AGUA) return 0;
    } else return 0;
    return 1;
}

void colocar_navio(Tabuleiro *t, int linha, int coluna, int tamanho, char orientacao) {
    if (orientacao == 'H')
        for (int j = 0; j < tamanho; j++)
            t->grade[linha][coluna + j] = NAVIO;
    else
        for (int i = 0; i < tamanho; i++)
            t->grade[linha + i][coluna] = NAVIO;
}

void posicionar_frota(Tabuleiro *t) {
    int linha, coluna, quantidade;
    char orientacao;
    char *navios_nome[] = {"Porta-avioes (5)", "Couracado (4)", "Cruzador (3)", "Submarino (3)", "Destroier (2)"};
    int navios_tamanho[] = {5, 4, 3, 3, 2};

    printf("Quantos navios deseja colocar? (1 a 5): ");
    scanf("%d", &quantidade);
    if (quantidade < 1) quantidade = 1;
    if (quantidade > 5) quantidade = 5;

    for (int n = 0; n < quantidade; n++) {
        int valido = 0;
        while (!valido) {
            mostrar_tabuleiro(t, 1);
            printf("\nEscolha o navio %d de %d:\n", n + 1, quantidade);
            for (int i = 0; i < 5; i++)
                printf("%d - %s\n", i + 1, navios_nome[i]);
            int escolha;
            printf("Digite o numero do navio: ");
            scanf("%d", &escolha);
            if (escolha < 1 || escolha > 5) {
                printf("Escolha invalida!\n");
                continue;
            }

            printf("Digite linha e coluna iniciais: ");
            scanf("%d %d", &linha, &coluna);
            printf("Digite orientacao (H para horizontal, V para vertical): ");
            scanf(" %c", &orientacao);

            if (validar_posicao(t, linha, coluna, navios_tamanho[escolha-1], orientacao)) {
                colocar_navio(t, linha, coluna, navios_tamanho[escolha-1], orientacao);
                valido = 1;
            } else {
                printf("Posicao invalida! Tente novamente.\n");
            }
        }
        limpar_tela();
    }
}

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
    for (int i = 0; i < TAM; i++)
        for (int j = 0; j < TAM; j++)
            if (t->grade[i][j] == NAVIO) return 0;
    return 1;
}

int jogar_turno(Tabuleiro *atacante, Tabuleiro *defensor, char *nomeAtacante, char *nomeDefensor) {
    int linha, coluna;
    int resultado = -1;

    printf("\nVez de %s\n", nomeAtacante);
    mostrar_tabuleiro(defensor, 0);

    while (resultado == -1) {
        printf("Digite linha e coluna para atacar: ");
        scanf("%d %d", &linha, &coluna);
        resultado = atacar(defensor, linha, coluna);
    }

    mostrar_tabuleiro(defensor, 0);

    if (verificar_vitoria(defensor)) {
        mostrar_vencedor(nomeAtacante);
        return 1;
    }

    printf("\nPressione Enter para passar a vez para %s...", nomeDefensor);
    getchar(); getchar(); // pausa para próximo jogador
    limpar_tela();

    return 0;
}

void mostrar_vencedor(char *nomeVencedor) {
    printf("\nParabéns, %s! Você venceu!\n\n", nomeVencedor);
    printf("        ___________\n");
    printf("       '._==_==_=_.'\n");
    printf("       .-\\:      /-.\n");
    printf("      | (|:.     |) |\n");
    printf("       '-|:.     |-'\n");
    printf("         \\::.    /\n");
    printf("          '::. .'\n");
    printf("            ) (\n");
    printf("          _.' '._\n");
    printf("         `\"\"\"\"\"\"\"`\n");
    printf(" Parabéns, Campeão!\n\n");
}

void jogar_partida(Tabuleiro *j1, Tabuleiro *j2) {
    int venceu = 0;
    while (!venceu) {
        venceu = jogar_turno(j1, j2, "Jogador 1", "Jogador 2");
        if (venceu) break;
        venceu = jogar_turno(j2, j1, "Jogador 2", "Jogador 1");
    }
}

void limpar_tela() {
    system("clear || cls");
}

void menu_inicial() {
 printf("====================================================================\n");
 printf("   ____        __        ____             _   __                  __\n");
 printf("  / __ )____ _/ /_____ _/ / /_  ____ _   / | / /___ __   ______ _/ /\n");
 printf(" / __  / __ `/ __/ __ `/ / __ \\/ __ `/  /  |/ / __ `/ | / / __ `/ / \n");
 printf("/ /_/ / /_/ / /_/ /_/ / / / / / /_/ /  / /|  / /_/ /| |/ / /_/ / /  \n");
 printf("\\____/\\__,_/\\__/\\__,_/_/_/ /_/\\__,_/  /_/ |_/\\__,_/ |___/\\__,_/_/   \n");
 printf("====================================================================\n");
 printf("\n1 - Iniciar Jogo\n0 - Sair\nEscolha: ");
}

int main() {
    int opcao;
    menu_inicial();
    scanf("%d", &opcao);
    if (opcao != 1) {
        printf("Saindo do jogo...\n");
        return 0;
    }

    Tabuleiro jogador1, jogador2;
    inicializar_tabuleiro(&jogador1);
    inicializar_tabuleiro(&jogador2);

    printf("\nJogador 1, posicione sua frota:\n");
    posicionar_frota(&jogador1);

    printf("\nJogador 2, posicione sua frota:\n");
    posicionar_frota(&jogador2);

    limpar_tela();
    jogar_partida(&jogador1, &jogador2);

    return 0;
}