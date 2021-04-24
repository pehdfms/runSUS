#ifndef SAIR_H
#define SAIR_H

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

void sair();
int teclasMenuSair(int *posicao);

// TODO: Separar a escrita das setas no menu para uma funcao separada

void sair() {
    // Desenha o menu de sair
    const int VOLTAR = 0, FECHAR = 1;
    int POSICAO = VOLTAR;

    while(1) {
        system("cls");

        printf("###########################################################################\n");
        printf("\n\n                                                                       \n");
        printf("                     VOCE TEM CERTEZA QUE DESEJA SAIR?                     \n");
        printf("                                                                           \n");

        printf("\t\t\t\t");
        if (POSICAO == VOLTAR) printf(">");
        printf("VOLTAR\n");

        printf("\t\t\t\t");
        if (POSICAO == FECHAR) printf(">");
        printf("SAIR\n");
        printf("\n\n                                                                       \n");
        printf("###########################################################################\n");

        // Pega tecla do usuario
        // Se a funcao retornar 1, significa que usuario pressionou VOLTAR
        // e devemos voltar para o menu inicial
        if (teclasMenuSair(&POSICAO) == 1) return;
    }
}

int teclasMenuSair(int *posicao) {
    // Recebe e cuida das teclas usadas pelo usuario no menu sair
    const int VOLTAR = 0, FECHAR = 1; // Posicoes dos botoes, maior = mais baixo na tela

    switch(getch()) {
        case KEY_DOWN:
            // Se o jogador pressionou para baixo, move sua opcao para baixo
            (*posicao)++;
            if (*posicao > 1) *posicao = 0; // Se estiver na ultima opcao, volta a primeira
            break;
        case KEY_UP:
            // Se o jogador pressionou para baixo, move sua opcao para baixo
            (*posicao)--;
            if (*posicao < 0) *posicao = 1; // Se estiver na ultima opcao, volta a primeira
            break;
        case KEY_ENTER:
            // Se clicar ENTER, aplicamos a funcao que ele tiver selecionado
            // Retornamos 1 quando voltar para que sair() saiba que tem que voltar tambem
            if (*posicao == VOLTAR) return 1;

            // Se ele tiver selecionado para fechar
            if (*posicao == FECHAR) {
                system("cls");      // Limpamos a tela
                printf("Tchau!\n"); // Escrevemos tchau
                exit(0);            // Saimos do programa
            }
            break;
        case KEY_ESC:
            // Tratamos ESC como se o usuario tivesse clicado em VOLTAR normalmente
            return 1;
            break;
    }

    return 0;
}

/* void desenharSetasMenuInicial(int posicao) { */
/*     // Desenha as setas no menu inicial quando o jogador se mexe */
/*     int posxPorOpcao[4] = {33, 33, 32, 34}; // Posicoes x onde a seta deve ser inserida */
/*     int posyPrimeiraOpcao = 6; // Posicao y da primeira opcao */

/*     // Percorre a quantidade de opcoes */
/*     for (int i = 0; i < 4; i++) { */
/*         // Coloca o cursor na posicao aonde a seta deve ser inserida */
/*         setPosCursor(posxPorOpcao[i], posyPrimeiraOpcao + i); */

/*         // Se for a opcao selecionada, escreve >, se nao, limpa o > que ta ali (troca por espaco) */
/*         if (posicao == i) printf(">"); */
/*         else printf(" "); */
/*     } */
/* } */

#endif
