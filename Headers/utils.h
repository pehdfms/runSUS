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
void desenharSetas(int quantidadeOpcoes, int posicao, int posxPorOpcao[]);

void salvarArquivoRanking(Score ranking[]) {
    // Tenta salvar um arquivo ranking.txt para salvar o ranking
    FILE * arquivo;

    arquivo = fopen("ranking.txt", "w");

    // Se nao conseguir abrir ranking.txt, desista
    if (arquivo == NULL) {
        printf("NAO FOI POSSIVEL ABRIR RANKING\n");
        exit(1);
    }

    // Escreve as 10 pontuacoes do ranking para o arquivo
    for (int i = 0; i < 10; i++) {
        fprintf(arquivo, "%s %d\n", ranking[i].nome, ranking[i].pontuacao);
    }

    fclose(arquivo);
}

int lerArquivoRanking(Score ranking[]) {
    // Tenta ler um arquivo ranking.txt para criar o ranking
    FILE * arquivo;

    arquivo = fopen("ranking.txt", "r");

    // Se nao conseguir abrir ranking.txt, retorne 1 para podermos gerar um novo
    if (arquivo == NULL) {
        return 1;
    }

    // Scaneia as 10 pontuacoes do arquivo para criar nosso ranking
    for (int i = 0; i < 10; i++) {
        fscanf(arquivo, "%s %d", ranking[i].nome, &ranking[i].pontuacao);
    }

    fclose(arquivo);

    return 0;
}

void inicializarPlacar(Score ranking[]) {
    // Inicializa a lista de ranking com Scores padrao
    // nome = ####
    // pontuacao = 0
    char nomePadrao[] = "####";

    // Percorre a lista colocando os scores criados
    for (int i = 0; i < 10; i++) {
        ranking[i] = criarScore(nomePadrao, 0);
    }
}

void setPosCursor(short int x, short int y) {
    // Coloca o cursor do terminal nas posicoes x e y
    HANDLE winHandle;
    COORD pos = {x, y};
    winHandle = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleCursorPosition(winHandle, pos);
}

void ordenarScores(Score listaScores[]) {
    // Ordena a lista de ranking da maior pontuacao para a menor
    int inicio = 0; // inicio da lista
    int fim = 10;   // tamanho da lista

    // Percorre a lista
    for (int i = inicio+1; i < fim; i++) {
        // Comecando de i, percorremos ate o inicio da lista
        // e enquanto ela nao estiver ordenada (lista[j-1] for maior do que lista[j])
        for (int j = i; j > inicio && listaScores[j-1].pontuacao < listaScores[j].pontuacao; j--) {
            // Quando tivermos 2 items desordenados, trocamos seus valores entre si
            Score temp = listaScores[j-1];
            listaScores[j-1] = listaScores[j];
            listaScores[j] = temp;
            // E continuamos indo ate o inicio da lista, ordenando o que estiver no caminho
        }
    }
}

Score criarScore(char nome[5], int pontuacao) {
    // Cria e retorna um score baseado no nome e pontuacao dados
    Score novoScore; // Define o novo score

    strcpy(novoScore.nome, nome);    // Copia o nome recebido para o nome no Score
    novoScore.pontuacao = pontuacao; // Copia a pontuacao recebida para a pontuacao no Score

    return novoScore;
}

void desenharSetas(int quantidadeOpcoes, int posicao, int posxPorOpcao[]) {
    // Desenha as setas em menus de opcao (e.g Inicial, Sair)
    // posxPorOpcao e a posicao x aonde deve ser inserida a seta para cada opcao recebida
    // Posicao y da primeira opcao
    const int posyPrimeiraOpcao = 6; // Sempre 6 por enquanto, possivel argumento depois?

    // Percorre a quantidade de opcoes
    for (int i =0; i < quantidadeOpcoes; i++) {
        // Coloca o cursor na posicao aonde a seta deve ser inserida
        setPosCursor(posxPorOpcao[i], posyPrimeiraOpcao+i);

        // Se for a opcao selecionada escrevemos >, se nao, limpamos o > que pode estar ali
        if (posicao == i) printf(">");
        else printf(" ");
    }
}

void moverPosicaoMenu(int tecla, int quantidadeOpcoes, int * posicao) {
    // Recebe uma tecla do usuario e utiliza ela para alterar a posicao
    switch(tecla) {
        case KEY_DOWN:
            // Se ele clicou seta para baixo
            (*posicao)++; // Movimentamos para baixo (maior = mais para baixo)

            // Se estivermos na ultima opcao, voltamos ao inicio
            if (*posicao > quantidadeOpcoes-1) *posicao = 0;
            break;
        case KEY_UP:
            // Se ele clicou seta para cima
            (*posicao)--; // Movimentamos para cima (menor = mais para cima)

            // Se estivermos na primeira opcao, mudamos para a ultima
            if (*posicao < 0) *posicao = quantidadeOpcoes-1;
            break;
    }
}

#endif
