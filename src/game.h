#ifndef GAME_H
#define GAME_H

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#include "utils.h"

int eLetra(char c);
void pedirNome(char nome[5]);
void iniciarJogo(Score ranking[]);
void desenharPontuacao(int pontuacao);
void desenharTempo(int tempoRestante);
void desenharJogador(int y);
void adicionarScore(Score ranking[], Score novoScore);

#endif
