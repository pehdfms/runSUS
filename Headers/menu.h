#ifndef MENU_H
#define MENU_H

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "creditos.h"
#include "game.h"
#include "placar.h"
#include "sair.h"
#include "utils.h"

void desenharTelaInicial();
void menuInicial(Score ranking[]);
void teclasMenuInicial(int tecla, int posicao, Score ranking[]);

#endif
