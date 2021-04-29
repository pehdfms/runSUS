#include "game.h"

int eLetra(char c) {
    // Retorna um se o caractere c for uma letra (a-z/A-Z)
    // valores correspondem a tabela ascii
    if ((c > 64 && c < 91) || (c > 96 && c < 123)) return 1;
    return 0;
}

void pedirNome(char nome[5]) {
    // Pede o nome do usuario para comecar o jogo
    system("cls");

    printf("Escreva seu nome: \n");

    printf("\e[?25h"); // Mostra o cursor

    // Espera o jogador inserir 4 letras
    int letraAtual = 0;
    while (letraAtual < 4) {
        int letra = getch();

        // Se o usuario pressionou o delete e nao estiver na primeira letra
        // voltamos para a letra anterior e deixamos ele troca-la por outra
        if (letra == 8 && letraAtual > 0) {
            printf("\b"); // \b move o cursor para tras 1 vez
            letraAtual--; // Diminuimos a quantidade de letras "confirmadas" por um
            continue;     // E pedimos por outra letra de novo
        }

        // Se o usuario pressionou algo que nao seja uma letra, ignoramos
        if (!eLetra(letra)) continue;

        nome[letraAtual] = (char) letra; // Setamos a letra que o usuario escolheu para o seu nome
        printf("%c", letra); // e escrevemos a letra que ele escolheu

        letraAtual++; // Passamos para pedir a proxima letra, se ja pegamos 4, terminamos
    }
    nome[4] = '\0'; // caractere terminador

    printf("\e[?25l"); // Esconde o cursor
}

void iniciarJogo(Score ranking[]) {
    // Loop principal do jogo, desenhando o e implementando a logica do jogo
    int posJogador = 2; // Posicao y do jogador nas lanes (0 = primeira, 2 = meia, 4 = ultima)
    int tempoRestante = 60;
    int pontuacao = 500;

    // Recebe o nome do usuario para criar seu Score
    char nome[5];
    pedirNome(nome);

    Score novoScore;

    // Desenha a tela
    system("cls");

    printf("##############################################################################\n");
    printf("                                                                             #\n");
    printf("---------------------------------------------------------------------------  #\n");
    printf("---------------------------------------------------------------------------  #\n");
    printf("!--------------------------------------------------------------------------  #\n");
    printf("---------------------------------------------------------------------------  #\n");
    printf("---------------------------------------------------------------------------  #\n");
    printf("                                                                             #\n");
    printf("PONTUACAO : 0                         |                           TEMPO : 0  #\n");
    printf("##############################################################################\n");

    while(1) {
        // Substitui os '0's dos indicadores com seu valor correspondente
        desenharPontuacao(pontuacao);
        desenharTempo(tempoRestante);

        switch(getch()) {
            case KEY_DOWN:
                // Se o jogador se moveu para baixo, atualizamos sua posicao
                posJogador++;

                if (posJogador > 4) posJogador = 4; // Se ja estiver na ultima lane, mantem posicao
                else desenharJogador(posJogador);   // Se ele realmente se moveu (nao manteve posicao), desenha sua nova posicao
                break;
            case KEY_UP:
                // Se o jogador se moveu para cima, atualizamos sua posicao
                posJogador--;

                if (posJogador < 0) posJogador = 0; // Se ja estiver na primeira lane, mantem posicao
                else desenharJogador(posJogador);   // Se ele realmente se moveu (nao manteve posicao), desenha sua nova posicao
                break;
            case KEY_SPACE:
                // Aumenta a pontuacao por 50 toda vez que apertamos ESPACO
                // Temporario para testar ranking, trocaremos por atirar projetil depois
                pontuacao += 50;
                break;
            case KEY_ESC:
                // Se pressionarmos ESC, salva a nova pontuacao e adiciona ao ranking
                // e saimos para o menu inicial
                // Provavelmente uma funcao temporaria, sua logica sera usada para a funcao de morte do jogador
                novoScore = criarScore(nome, pontuacao);
                adicionarScore(ranking, novoScore);

                // Salva ao sair
                salvarArquivoRanking(ranking);
                return;
        }
    }
}

void desenharPontuacao(int pontuacao){
    // Escreve a pontuacao do jogador na tela do jogo
    const int posxPontuacao = 12; // Posicao x do numeral em PONTUACAO : x
    const int posyPontuacao = 8;  // Posicao y do indicador de pontuacao na tela

    setPosCursor(posxPontuacao, posyPontuacao); // Colocamos o cursor na posicao do indicador de pontuacao
    printf("%d", pontuacao); // e escrevemos a pontuacao do jogador
}

void desenharTempo(int tempoRestante){
    // Escreve o tempo restante na tela do jogo
    const int posxTempo = 74; // Posicao x do numeral em TEMPO : x
    const int posyTempo = 8;  // Posicao y do indicador de tempo na tela

    setPosCursor(posxTempo, posyTempo); // Colocamos o cursor na posicao do indicador de tempo
    printf("%d", tempoRestante); // e escrevemos o tempo restante
}

void desenharJogador(int posy) {
    // Funcao para desenhar o ! na posicao do jogador
    // Atravessa as posicoes 2 - 6 (as lanes do jogo)
    for (int i = 2; i < 7; i++) {
        // Coloca o cursor na posicao (x, y) = (0, lane)
        setPosCursor(0, i);

        // Se o jogador estiver na posicao, desenha !, se nao, desenha -
        if (i == (2 + posy)) printf("!");
        else printf("-");
    }
}

void adicionarScore(Score ranking[], Score novoScore) {
    // Adiciona uma pontuacao para a lista de ranking
    // Se a pior pontuacao do placar for menor que a nova pontuacao
    // trocamos ela pela pontuacao nova e reordenamos a lista
    if (novoScore.pontuacao > ranking[9].pontuacao) {
        ranking[9] = novoScore;
        ordenarScores(ranking);
    }
}
