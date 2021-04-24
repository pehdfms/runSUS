#ifndef SAIR_H
#define SAIR_H

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

void sair();
int teclasMenuSair(int tecla, int posicao);

void sair() {
    // Escreve o menu sair e pega interacao com o usuario
    const int quantidadeOpcoes = 2; // Quantidade de opcoes
    int posxPorOpcao[2] = {33, 34}; // Posicoes x de setas nas opcoes do menu
    int POSICAO = 0; // Posicao da seta do usuario no menu

    system("cls");

    printf("###########################################################################\n");
    printf("\n\n                                                                       \n");
    printf("                     VOCE TEM CERTEZA QUE DESEJA SAIR?                     \n");
    printf("                                                                           \n");
    printf("                                 >VOLTAR                                   \n");
    printf("                                   SAIR                                    \n");
    printf("\n\n                                                                       \n");
    printf("###########################################################################\n");

    while(1) {
        int tecla = getch(); // Pega tecla do usuario

        // Move sua posicao no menu dependendo de que botao clicou
        moverPosicaoMenu(tecla, quantidadeOpcoes, &POSICAO);

        // Interacao com os botoes do menu
        // teclasMenuSair retorna 1 quando o jogador quer voltar ao menu inicial
        // portanto devemos esperar esse 1 e retornar aqui tambem se o recebemos
        int devemosRetornar = teclasMenuSair(tecla, POSICAO);
        if (devemosRetornar) return;

        // Desenha uma seta na opcao escolhida
        desenharSetas(POSICAO, quantidadeOpcoes, posxPorOpcao);

    }
}

int teclasMenuSair(int tecla, int posicao) {
    // Recebe e cuida das teclas usadas pelo usuario no menu sair
    const int VOLTAR = 0, FECHAR = 1; // Posicoes dos botoes, maior = mais baixo na tela

    switch(tecla) {
        case KEY_ENTER:
            // Se clicar ENTER, aplicamos a funcao que ele tiver selecionado
            // Retornamos 1 quando voltar para que sair() saiba que tem que voltar tambem
            if (posicao == VOLTAR) return 1;

            // Se ele tiver selecionado para fechar
            if (posicao == FECHAR) {
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

#endif
