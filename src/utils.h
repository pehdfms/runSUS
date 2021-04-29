#ifndef UTILS_H
#define UTILS_H

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_ENTER 13
#define KEY_ESC 27
#define KEY_SPACE 32

#include <stdio.h>
#include <conio.h>
#include <windows.h>

typedef struct Scores {
    // Estrutura de pontuacao
    char nome[5];  // Nome de 4 caracteres (+ terminador)
    int pontuacao; // Pontuacao (deve caber em int)
} Score;

Score criarScore(char nome[5], int pontuacao);
void setPosCursor(short int x, short int y);
void ordenarScores(Score listaScores[]);
void inicializarPlacar(Score ranking[]);
int lerArquivoRanking(Score ranking[]);
void salvarArquivoRanking(Score ranking[]);
void moverPosicaoMenu(int tecla, int quantidadeOpcoes, int * posicao);
void desenharSetas(int posicao, int quantidadeOpcoes, int posxPorOpcao[]);

#endif
