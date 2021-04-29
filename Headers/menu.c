#include "menu.h"

void desenharTelaInicial() {
    // Limpa e desenha a tela estatica do menu inicial
    system("cls");

    printf("###########################################################################\n");
    printf("\n\n                                                                       \n");
    printf("                                  runSUS                                   \n");
    printf("                                                                           \n");
    printf("                                 >COMECE                                   \n");
    printf("                                  PLACAR                                   \n");
    printf("                                 CREDITOS                                  \n");
    printf("                                   SAIR                                    \n");
    printf("\n\n                                                                       \n");
    printf("###########################################################################\n");
}

void menuInicial(Score ranking[]) {
    // Escreve o menu inicial e pega interacao com o usuario
    const int quantidadeOpcoes = 4; // Quantidade de opcoes
    int posxPorOpcao[4] = {33, 33, 32, 34}; // Posicoes x onde a seta deve ser inserida
    int POSICAO = 0; // Posicao da seta do usuario no menu

    // Desenha a tela
    desenharTelaInicial();

    while (1) {
        // Pega tecla do usuario
        int tecla = getch();

        // Move sua posicao na tela
        moverPosicaoMenu(tecla, quantidadeOpcoes, &POSICAO);

        // Entra no menu selecionado
        teclasMenuInicial(tecla, POSICAO, ranking);

        // Desenha as setas de selecao no menu
        desenharSetas(POSICAO, quantidadeOpcoes, posxPorOpcao);
    }
}

void teclasMenuInicial(int tecla, int posicao, Score ranking[]) {
    // Recebe e cuida das teclas usadas pelo usuario no menu inicial
    if (tecla == KEY_ENTER) {
        // Posicoes dos botoes no menu
        const int INICIO = 0, PLACAR = 1, CREDITOS = 2, SAIR = 3;

        // Se clicar ENTER, movemos para o menu especifico baseado no que estiver selecionado
        if (posicao == INICIO) iniciarJogo(ranking);
        if (posicao == PLACAR) menuPlacar(ranking);
        if (posicao == CREDITOS) menuCreditos();
        if (posicao == SAIR) sair();

        // Reescreve a tela inicial quando retornamos nela
        desenharTelaInicial();
    }
}
