#include "placar.h"

void menuPlacar(Score ranking[]) {
    // Desenha a tela do placar
    system("cls");

    // Escreve a tela de placar, # representa nenhum valor (ou um valor que sera preenchido)
    printf("###########################################################################\n");
    printf("\n\n                                                                       \n");
    printf("                            PLACAR - TOP 10                                \n");
    printf("                             1 - #### | #                                  \n");
    printf("                             2 - #### | #                                  \n");
    printf("                             3 - #### | #                                  \n");
    printf("                             4 - #### | #                                  \n");
    printf("                             5 - #### | #                                  \n");
    printf("                             6 - #### | #                                  \n");
    printf("                             7 - #### | #                                  \n");
    printf("                             8 - #### | #                                  \n");
    printf("                             9 - #### | #                                  \n");
    printf("                            10 - #### | #                                  \n");
    printf("                                                                           \n");
    printf("                  PRESSIONE QUALQUER BOTAO PARA RETORNAR                   \n");
    printf("\n\n                                                                       \n");
    printf("###########################################################################\n");

    desenharScores(ranking); // Preenche os #### na tela baseado na lista de ranking

    getch(); // Espera o jogador pressionar algum botao para retornar
}

void desenharScores(Score listaScores[]) {
    // Desenha as ranking no lugar de #### no menu do placar
    const int posxNome1 = 33;      // Posicao x de onde comeca o nome quando mostrado no terminal
    const int posyNome1 = 5;       // Posicao y de onde comeca o nome quando mostrado no terminal
    const int offsetNomePonto = 7; // Diferenca de posicao x do inicio do nome no terminal com o inicio da pontuacao

    // Primeiro ordenamos a lista para que as maiores ranking aparecam primeiro
    ordenarScores(listaScores);

    for (int i = 0; i < 10; i++) {
        // Se a pontuacao for 0, significa que o Score so ta enchendo a lista, entao nao escrevemos nada.
        // Como a lista esta ordenada, quando chegamos a 0 a gente ja pode parar de escrever.

        // Colocamos o cursor na posicao do nome e printamos o nome
        setPosCursor(posxNome1, (posyNome1 + i));
        printf("%s", listaScores[i].nome);

        // Colocamos o cursor na posicao da pontuacao e printamos a pontuacao
        setPosCursor(posxNome1+offsetNomePonto, (posyNome1 + i));
        printf("%d", listaScores[i].pontuacao);
    }
}
