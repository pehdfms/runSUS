#include <stdio.h>

#include "Headers/menu.h"
#include "Headers/utils.h"

int main() {
    // Esconde o cursor
    printf("\e[?25l");

    // Inicializa o placar
    Score pontuacoes[10];

    // lerArquivoRanking retorna 1 se nao conseguiu abrir o arquivo
    if (lerArquivoRanking(pontuacoes)) {
        // Se nao conseguimos ler, inicializamos o placar com valores padroes
        inicializarPlacar(pontuacoes);

        // E salvamos esse placar no arquivo
        salvarArquivoRanking(pontuacoes);
    }

    // Inicia o menu inicial
    menuInicial(pontuacoes);
}
