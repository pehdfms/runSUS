/*
d888888b d8b   db d888888b  .o88b. d888888b  .d8b.  db      d888888b d88888D  .d8b.   .o88b.  .d8b.   .d88b.
  `88'   888o  88   `88'   d8P  Y8   `88'   d8' `8b 88        `88'   YP  d8' d8' `8b d8P  Y8 d8' `8b .8P  Y8.
   88    88V8o 88    88    8P         88    88ooo88 88         88       d8'  88ooo88 8P      88ooo88 88    88
   88    88 V8o88    88    8b         88    88~~~88 88         88      d8'   88~~~88 8b      88~~~88 88    88
  .88.   88  V888   .88.   Y8b  d8   .88.   88   88 88booo.   .88.    d8' db 88   88 Y8b  d8 88   88 `8b  d8'
Y888888P VP   V8P Y888888P  `Y88P' Y888888P YP   YP Y88888P Y888888P d88888P YP   YP  `Y88P' YP   YP  `Y88P'
*/

// Bibliotecas
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Arduino_ST7789_Fast.h>
#include <EEPROM.h>

// Constantes
#define CS_ALWAYS_LOW // Ativa otimizacoes na biblioteca ST7789_FAST
#define TFT_DC  7     // Pino DC do display
#define TFT_RST 8     // Pino RST do display

#define CIMA_BUTTON 4  // Botao de cima
#define BAIXO_BUTTON 3 // Botao de baixo
#define ENTER_BUTTON 2 // Botao de enter

#define MAX_ENTIDADES 50 // Quantidade maxima de entidades para controlar no jogo

// Tipos de entidades
#define DEFAULTENTIDADE -1
#define PLAYER 0
#define ENEMY 1
#define TIRO 2
#define PILULA 3

// Cores para as sprites
const PROGMEM unsigned int C0 = RED; // Vermelho é tratado como a cor de transparencia, ao invez de desenharmos essa cor, desenhamos o a cor do background
const PROGMEM unsigned int C1 = 0x4040;
const PROGMEM unsigned int C2 = 0xFCB2;
const PROGMEM unsigned int C3 = BLUE;
const PROGMEM unsigned int C4 = CYAN;
const PROGMEM unsigned int C5 = BLACK;
const PROGMEM unsigned int C6 = WHITE;
const PROGMEM unsigned int C7 = YELLOW;
const PROGMEM unsigned int C8 = 0xb723;
const PROGMEM unsigned int C9 = 0x2589;

// Sprites
const PROGMEM unsigned int spriteJogador[] = {
    C0, C0, C1, C1, C1, C1, C0, C0,
    C1, C1, C1, C1, C1, C1, C1, C1,
    C1, C1, C1, C1, C1, C1, C1, C1,
    C1, C1, C2, C2, C2, C2, C1, C1,
    C1, C2, C5, C2, C2, C5, C2, C1,
    C1, C2, C2, C2, C2, C2, C2, C1,
    C0, C2, C2, C6, C6, C2, C2, C1,
    C0, C0, C2, C2, C2, C2, C0, C0,
    C0, C0, C0, C2, C2, C0, C0, C0,
    C0, C0, C3, C2, C2, C3, C0, C0,
    C4, C3, C3, C4, C4, C3, C3, C4,
    C4, C3, C3, C4, C4, C3, C3, C4,
    C4, C3, C3, C4, C4, C3, C3, C4,
    C2, C3, C3, C4, C4, C3, C3, C2,
    C2, C3, C3, C4, C4, C3, C3, C2,
    C2, C3, C3, C6, C6, C3, C3, C2,
    C0, C6, C6, C6, C6, C6, C6, C0,
    C0, C6, C6, C0, C0, C6, C6, C0,
    C0, C6, C6, C0, C0, C6, C6, C0,
    C1, C1, C1, C0, C0, C1, C1, C1,
};

const PROGMEM unsigned int spriteTiro[] = {
    C3, C3, C4, C4, C3, C3, C3, C0, C0, C0, C0,
    C0, C3, C4, C4, C4, C4, C4, C3, C3, C0, C0,
    C0, C0, C4, C4, C3, C4, C4, C4, C3, C3, C0,
    C0, C0, C3, C3, C3, C4, C4, C3, C3, C3, C3,
    C0, C0, C4, C4, C4, C4, C4, C4, C4, C4, C3,
    C0, C0, C4, C4, C4, C4, C4, C4, C3, C3, C0,
    C0, C3, C3, C4, C4, C4, C4, C4, C3, C0, C0,
    C3, C3, C4, C4, C3, C3, C3, C3, C0, C0, C0,
};

const PROGMEM unsigned int spritePilula[] = {
    C0, C0, C5, C5, C5, C5, C5, C5, C5, C5, C5, C5, C5, C0, C0,
    C0, C5, C7, C7, C7, C7, C7, C5, C3, C3, C3, C3, C3, C5, C0,
    C5, C7, C7, C7, C7, C7, C7, C5, C3, C3, C3, C3, C3, C3, C5,
    C5, C7, C7, C7, C7, C7, C7, C5, C3, C3, C3, C3, C3, C3, C5,
    C5, C7, C7, C7, C7, C7, C7, C5, C3, C3, C3, C3, C3, C3, C5,
    C0, C5, C7, C7, C7, C7, C7, C5, C3, C3, C3, C3, C3, C5, C0,
    C0, C0, C5, C5, C5, C5, C5, C5, C5, C5, C5, C5, C5, C0, C0,
};

const PROGMEM unsigned int spriteVirus[] = {
    C0, C0, C0, C0, C0, C8, C8, C9, C9, C8, C0, C0, C0, C0, C0,
    C0, C0, C9, C8, C8, C8, C8, C8, C9, C8, C8, C8, C9, C0, C0,
    C0, C0, C9, C9, C8, C8, C8, C8, C8, C8, C8, C8, C9, C0, C0,
    C9, C8, C8, C8, C8, C9, C8, C8, C8, C8, C8, C8, C8, C8, C0,
    C9, C9, C8, C8, C8, C9, C9, C8, C8, C9, C8, C8, C8, C8, C9,
    C8, C8, C8, C8, C8, C8, C8, C8, C8, C9, C8, C8, C8, C9, C9,
    C8, C8, C8, C8, C8, C8, C8, C8, C8, C8, C8, C8, C8, C8, C8,
    C8, C8, C9, C9, C8, C9, C9, C8, C9, C8, C8, C8, C9, C8, C8,
    C9, C9, C8, C8, C8, C9, C8, C8, C9, C8, C9, C9, C8, C8, C8,
    C9, C8, C8, C8, C8, C8, C8, C8, C8, C8, C8, C8, C8, C8, C8,
    C0, C8, C8, C8, C8, C8, C9, C9, C8, C8, C8, C8, C8, C9, C9,
    C0, C8, C8, C9, C8, C8, C8, C8, C8, C8, C8, C8, C8, C8, C0,
    C0, C9, C9, C9, C8, C8, C8, C9, C8, C8, C8, C9, C8, C0, C0,
    C0, C0, C0, C8, C8, C8, C9, C9, C8, C9, C8, C9, C9, C9, C0,
    C0, C0, C0, C0, C0, C8, C9, C8, C8, C8, C9, C0, C0, C0, C0,
};

// Entidade usada para os objetos "desenhaveis" do jogo
typedef struct entity {
    int tipo;

    int x, y, oldx, oldy;
    int altura, largura;
    int velocidade;

    const unsigned int *sprite;
} entity;

// Struct que segura as pontuacoes {pontuacao, nome} do jogador
typedef struct pontuacao {
    int pontos;
    char nome[5];
} pontuacao;

// Lista de pontuacoes
pontuacao listaPontos[10];

// Tela
Arduino_ST7789 tft = Arduino_ST7789(TFT_DC, TFT_RST);

// Funcoes
void wait();
void pedirNome(char nome[5]);
void gameOver(int pontos);
void ordenarPontos();
void menuPlacar();
void elimineDesenho(entity entidade);
void desenhaEntidade(entity entidade);
void menuCreditos();
void atualizarEntidades(entity entidades[MAX_ENTIDADES], int *pontos, float *tempoRestante, int *tirosRestantes);
int gerenciarColisoes(int pos_atual, entity atual, entity entidades[MAX_ENTIDADES], int *pontos, float *tempoRestante, int *tirosRestantes);
pontuacao jogo();
int existeTiro(entity entidades[MAX_ENTIDADES]);
int encontreLivre(entity entidades[MAX_ENTIDADES]);
int encontre(entity entidades[MAX_ENTIDADES], int tipo);
entity criarVirus();
entity criarPilula();
entity criarJogador();
entity atirarBala(entity jogador);
entity entidadeDefault();
void displayBackgroundJogo();
int corBackgroundJogo(int y);
void textomenuinicial();
void menu_inicial();
void print_cursor(int pos_antiga, int pos);
void centralize(int size_fonte, int y0, int tamanhopalavra);
void displayBackgroundMenu();
void displayTempo(int antigo, int atual);
void displayBalas(int antigo, int atual);
void displayPontos(int antigo, int atual);
void displayDinamico(int antigo, int atual, int posx, int posy, int tamanho);
void displayHUDEstatico();
int corBackgroundMenu(int y);
void lerEEPROM();
void salvarEEPROM();

void setup(void) {
    // Inicia a tela
    tft.begin();
    tft.setRotation(2);

    // Ativa os botoes
    digitalWrite(ENTER_BUTTON, HIGH);
    digitalWrite(CIMA_BUTTON, HIGH);
    digitalWrite(BAIXO_BUTTON, HIGH);

    // Le um pino desconectado para fazer o random dar numeros aleatorios
    randomSeed(analogRead(A0));

    // Inicializa a lista de pontuacoes
    // resetEEPROM(); // Temporario, recompile na apresentacao sem essa linha
    lerEEPROM();
    salvarEEPROM();

    // Entra no menu inicial
    menu_inicial();
}

void loop() {
}

int corBackgroundMenu(int y) {
    int corAtual = tft.Color565(y, 0, 0);
    return corAtual;
}

void resetEEPROM() {
  for (int i = 0; i < 512; i++) {
    EEPROM.write(i, 255);
  }
}

void lerEEPROM() {
  // Se o EEPROM nao estiver escrito ainda, deixamos valores default
  if (EEPROM.read(0) == 255) {
    pontuacao ruiz = {10075, "RUIZ"}; // LEMBRAREMOS DO BRABO
    listaPontos[0] = ruiz;
    for (int i = 1; i < 10; i++) {
        pontuacao novaPontuacao = {-1, "    "};
        listaPontos[i] = novaPontuacao;
    }
    return;
  }
  
  for (int i = 0; i < 10; i++) {
    pontuacao pontuacaoLida;
    int endereco = i * sizeof(pontuacao);

    EEPROM.get(endereco, pontuacaoLida);
    listaPontos[i] = pontuacaoLida;
  }
}

void salvarEEPROM() {
  for (int i = 0; i < 10; i++) {
    int endereco = i * sizeof(pontuacao);
    EEPROM.put(endereco, listaPontos[i]);
  }
}

void displayHUDEstatico() {
    tft.setTextSize(1);
    centralize(1, 10, 23);
    tft.print("Score: 0000 | Balas: 05");
    centralize(1, 230, 19);
    tft.print("Tempo Restante: 060");
}

void displayDinamico(int antigo, int atual, int posx, int posy, int tamanho) {
    if (atual == antigo) return;

    int corBorda = tft.Color565(60, 0, 0);

    for (int i = 0; i < tamanho; i++) {
        int n = atual % 10;
        int oldn = antigo % 10;

        if (n != oldn) {
            int offsetx = (tamanho - i - 1) * 6;
            char cn = n + '0';

            tft.drawChar(posx+offsetx, posy, cn, WHITE, corBorda, 1);
        }

        atual /= 10;
        antigo /= 10;
    }
}

void displayPontos(int antigo, int atual) {
    displayDinamico(antigo, atual, 99, 10, 4);
}

void displayBalas(int antigo, int atual) {
    displayDinamico(antigo, atual, 183, 10, 2);
}

void displayTempo(int antigo, int atual) {
    displayDinamico(antigo, atual, 163, 230, 3);
}

void displayBackgroundMenu() {
    for (int i = 0; i < 240; i++) {
        int corAtual = corBackgroundMenu(i);
        tft.drawFastHLine(0, i, 240, corAtual);
    }
    tft.drawRect(0, 0, 240, 240, GREEN);
}

void centralize(int size_fonte, int y0, int tamanhopalavra) {
    int x0 = 115 - (5.0 * size_fonte * tamanhopalavra / 2.0);
    tft.setCursor( x0, y0 );
}

void print_cursor(int pos_antiga, int pos) {
    int y_antigo = 120 + 40 * pos_antiga;
    int y = 120 + 40 * pos;

    for (int i = y_antigo; i < y_antigo + 21; i++) {
        tft.drawFastHLine(30, i, 18, corBackgroundMenu(i));
    }
    
    tft.setTextSize(3);
    tft.setCursor(30, y);
    tft.print(">");
}

void menu_inicial() {
    displayBackgroundMenu();
    textomenuinicial();

    int pos = 0;
    int cima_prev = 0;
    int baixo_prev = 0;

    print_cursor(pos, pos);
    while (1) {
        int enter_clicado = !digitalRead(ENTER_BUTTON);
        int cima_clicado = !digitalRead(CIMA_BUTTON);
        int baixo_clicado = !digitalRead(BAIXO_BUTTON);

        int pos_antiga = pos;

        if (cima_clicado && !cima_prev) pos--;
        if (baixo_clicado && !baixo_prev) pos++;

        if (pos_antiga != pos) {
            if (pos > 2) pos = 0;
            if (pos < 0) pos = 2;

            print_cursor(pos_antiga, pos);
        }

        if (enter_clicado) {
            if (pos == 0) {
                pontuacao novaPontuacao = jogo();
                if (listaPontos[9].pontos < novaPontuacao.pontos) {
                    listaPontos[9] = novaPontuacao;
                    ordenarPontos();
                    salvarEEPROM();
                }
            }
            if (pos == 1) {
                menuCreditos();
            }
            if (pos == 2) {
                menuPlacar();
            }

            displayBackgroundMenu();
            textomenuinicial();

            print_cursor(pos_antiga, pos);
            delay(200);
        }

        cima_prev = cima_clicado;
        baixo_prev = baixo_clicado;
    }
}

void textomenuinicial() {
    tft.setTextSize(4);
    centralize(4, 40, 6);
    tft.println("RunSUS");

    tft.setTextSize(3);
    centralize(3, 120, 6);
    tft.println("INICIE");

    centralize(3, 160, 8);
    tft.println("CREDITOS");

    centralize(3, 200, 6);
    tft.println("PLACAR");
}

int corBackgroundJogo(int y) {
    int cor = 40 + abs(120 - abs(y - 120));
    return tft.Color565(cor, 0, 0);
}

void displayBackgroundJogo() {
    int darkRed = tft.Color565(60, 0, 0);
    tft.fillRect(0, 0, 240, 20, darkRed);
    for (int i = 20; i < 220; i++) {
        int corAtual = corBackgroundJogo(i);
        tft.drawFastHLine(0, i, 240, corAtual);
    }
    tft.fillRect(0, 220, 240, 20, darkRed);
}

entity entidadeDefault() {
    int posx = 0;
    int posy = 0;
    int oldx = -100;
    int oldy = -100;
    int altura = 0;
    int largura = 0;
    int velocidade = 0;

    entity novaEntidade = {DEFAULTENTIDADE, posx, posy, oldx, oldy, altura, largura, velocidade, {}};
    return novaEntidade;
}

entity atirarBala(entity jogador) {
    int posx = jogador.x + jogador.largura + 5;
    int posy = jogador.y + jogador.altura / 2;
    int oldx = -100;
    int oldy = -100;
    int altura = 8;
    int largura = 11;
    int velocidade = 8;

    entity novaEntidade = {TIRO, posx, posy, oldx, oldy, altura, largura, velocidade, spriteTiro};
    return novaEntidade;
}

entity criarJogador() {
    int posx = 40;
    int posy = 120;
    int oldx = -100;
    int oldy = -100;
    int altura = 20;
    int largura = 8;
    int velocidade = 5;

    entity novaEntidade = {PLAYER, posx, posy, oldx, oldy, altura, largura, velocidade, spriteJogador};
    return novaEntidade;
}

entity criarPilula() {
    int posx = 239;
    int posy = random(41, 201);
    int oldx = -100;
    int oldy = -100;
    int altura = 7;
    int largura = 15;
    int velocidade = -4;

    entity novaEntidade = {PILULA, posx, posy, oldx, oldy, altura, largura, velocidade, spritePilula};
    return novaEntidade;
}

entity criarVirus() {
    int posx = 239;
    int posy = random(41, 201);
    int oldx = -100;
    int oldy = -100;
    int altura = 15;
    int largura = 15;
    int velocidade = -5;

    entity novaEntidade = {ENEMY, posx, posy, oldx, oldy, altura, largura, velocidade, spriteVirus};
    return novaEntidade;
}

int encontre(entity entidades[MAX_ENTIDADES], int tipo) {
    for (int i = 0; i < MAX_ENTIDADES; i++) {
        if (entidades[i].tipo == tipo) return i;
    }
    return -1;
}

int encontreLivre(entity entidades[MAX_ENTIDADES]) {
    return encontre(entidades, DEFAULTENTIDADE);
}

int existeTiro(entity entidades[MAX_ENTIDADES]) {
    return (encontre(entidades, TIRO) != -1);
}

pontuacao jogo() {
    displayBackgroundJogo();
    displayHUDEstatico();

    entity jogador = criarJogador();
    entity entidades[MAX_ENTIDADES];
    for (int i = 0; i < MAX_ENTIDADES; i++) { entidades[i] = entidadeDefault(); }

    int tirosRestantes = 5;
    float tempoRestante = 60;
    int pontos = 0;

    int antigoPontos = pontos;
    float antigoTempo = tempoRestante;
    int antigoTiros = tirosRestantes;

    unsigned long long int tempoDesdeUltimoTiro = millis();
    while (1) {
        int enter_clicado = !digitalRead(ENTER_BUTTON);
        int cima_clicado = !digitalRead(CIMA_BUTTON);
        int baixo_clicado = !digitalRead(BAIXO_BUTTON);

        if (cima_clicado) jogador.y -= jogador.velocidade;
        if (baixo_clicado) jogador.y += jogador.velocidade;
        jogador.y = constrain(jogador.y, 20, 200);

        if (enter_clicado && tirosRestantes && (millis()-tempoDesdeUltimoTiro) >= 300) {
            int espaco = encontreLivre(entidades);
            if (espaco != -1) {
                tempoDesdeUltimoTiro = millis();
                tirosRestantes--;

                entidades[espaco] = atirarBala(jogador);
            }
        }

        if (random(0, 200) == 0) {
            int espaco = encontreLivre(entidades);
            if (espaco != -1) {
                entidades[espaco] = criarPilula();
            }
        }

        if (random(0, 75) == 0) {
            int espaco = encontreLivre(entidades);
            if (espaco != -1) {
                entidades[espaco] = criarVirus();
            }
        }

        desenhaEntidade(jogador);
        atualizarEntidades(entidades, &pontos, &tempoRestante, &tirosRestantes);

        displayPontos(antigoPontos, pontos);
        displayBalas(antigoTiros, tirosRestantes);
        displayTempo(antigoTempo, tempoRestante);

        jogador.oldx = jogador.x;
        jogador.oldy = jogador.y;

        antigoPontos = pontos;
        antigoTiros = tirosRestantes;
        antigoTempo = tempoRestante;

        int morreu = gerenciarColisoes(-1, jogador, entidades, &pontos, &tempoRestante, &tirosRestantes);

        if (tempoRestante < 0 || morreu == -1) {
            gameOver(pontos);
            
            char nome[5] = "AAAA";
            pedirNome(nome);

            pontuacao novaPontuacao;
            novaPontuacao.pontos = pontos;
            strcpy(novaPontuacao.nome, nome);

            return novaPontuacao;
        }

        tempoRestante -= 16.0/1000.0;
        delay(16);
    }
}

int gerenciarColisoes(int pos_atual, entity atual, entity entidades[MAX_ENTIDADES], int *pontos, float *tempoRestante, int *tirosRestantes) {
    for (int i = pos_atual+1; i < MAX_ENTIDADES; i++) {
        entity outro = entidades[i];

        // Tipos das duas entidades
        int t_atual = atual.tipo;
        int t_outro = outro.tipo;

        // Continua se nao houve colisao
        if (atual.x+atual.largura < outro.x || atual.x > outro.x+outro.largura) continue;
        if (atual.y+atual.altura < outro.y || atual.y > outro.y+outro.altura) continue;

        int colisao = 0;
        if (t_atual == PLAYER) {
            if (t_outro == ENEMY) return -1;
            else if (t_outro == PILULA) {
                *tirosRestantes += 1;
                *tempoRestante += 5.0;
                if (*tirosRestantes > 5) {
                    *pontos += 50;
                    *tirosRestantes = 5;
                }
                else {
                    *pontos += 25;
                }
                colisao = 1;
            }
        }
        else if (t_atual == TIRO && t_outro == ENEMY ||
                 t_atual == ENEMY && t_outro == TIRO) {
                *pontos += 75;
                colisao = 1;
        }

        if (colisao) {
            if (pos_atual >= 0) {
                elimineDesenho(entidades[pos_atual]);
                entidades[pos_atual] = entidadeDefault();
            }

            elimineDesenho(entidades[i]);
            entidades[i] = entidadeDefault();
        }
    }

    return 0;
}

void atualizarEntidades(entity entidades[MAX_ENTIDADES], int *pontos, float *tempoRestante, int *tirosRestantes) {
    for (int i = 0; i < MAX_ENTIDADES; i++) {
        if (entidades[i].x < 0 || entidades[i].x > 240) {
            elimineDesenho(entidades[i]);
            entidades[i] = entidadeDefault();
        }
        if (entidades[i].tipo == DEFAULTENTIDADE) continue;

        entidades[i].x += entidades[i].velocidade;

        desenhaEntidade(entidades[i]);

        entidades[i].oldx = entidades[i].x;
        entidades[i].oldy = entidades[i].y;

        gerenciarColisoes(i, entidades[i], entidades, pontos, tempoRestante, tirosRestantes);
    }
}

void menuCreditos() {
    displayBackgroundMenu();

    tft.setTextSize(3);
    centralize(3, 40, 8);
    tft.print("CREDITOS");

    tft.setTextSize(2);
    char *nomes[] = {"Jean Clayton", "Pedro H. Garcia", "Kaik Felix", "Abel Chang", "Gabriel Ruiz", "Vinicius Menin"};
    int tamanhos[] = {12, 15, 10, 10, 12, 14};

    for (int posNome = 0; posNome < 6; posNome++) {
        centralize(2, 80 + 20 * posNome, tamanhos[posNome]);
        tft.println(nomes[posNome]);
    }

    wait();
}

void desenhaEntidade(entity entidade) {
    for (int x = 0; x < entidade.largura; x++) {
        for (int y = 0; y < entidade.altura; y++) {
            unsigned int corPixelAtual = pgm_read_word_near(entidade.sprite + x + (y * entidade.largura));
            if (corPixelAtual == RED) corPixelAtual = corBackgroundJogo(entidade.y + y);

            if (entidade.oldy + y < entidade.y || entidade.oldy + y > entidade.y + entidade.altura - 1
                    || entidade.oldx + x < entidade.x || entidade.oldx + x > entidade.x + entidade.largura - 1) {
                tft.drawPixel(entidade.oldx + x, entidade.oldy + y, corBackgroundJogo(entidade.oldy + y));
            }

            tft.drawPixel(entidade.x + x, entidade.y + y, corPixelAtual);
        }
    }
}

void elimineDesenho(entity entidade) {
    for (int y = 0; y < entidade.altura; y++) {
        int cor = corBackgroundJogo(entidade.y + y);
        tft.drawFastHLine(entidade.x, entidade.y+y, entidade.largura, cor);
    }
}

void menuPlacar() {
    displayBackgroundMenu();

    tft.setTextSize(3);
    centralize(3, 20, 6);
    tft.println("PLACAR");

    tft.setTextSize(2);
    for (int i = 0; i < 10; i++) {
        if (listaPontos[i].pontos == -1) break;

        centralize(2, 65+i*15, 10);
        tft.print(listaPontos[i].nome);
        tft.print(" | ");

        char spontos[5];
        sprintf(spontos, "%04d", listaPontos[i].pontos);
        tft.print(spontos);
    }

    wait();
}

void ordenarPontos() {
    for (int i = 1; i < 10; i++) {
        for (int j = i; j > 0 && listaPontos[j-1].pontos < listaPontos[j].pontos; j--) {
            pontuacao temp = listaPontos[j-1];
            listaPontos[j-1] = listaPontos[j];
            listaPontos[j] = temp;
        }
    }
}

void gameOver(int pontos) {
    tft.fillScreen(BLACK);

    tft.setTextSize(3);
    centralize(3, 80, 9);
    tft.print("GAME OVER");
    
    centralize(3,160,5);
    tft.print("SCORE");
    centralize(3,190,4);

    char spontos[5];
    sprintf(spontos, "%04d", pontos);
    tft.print(spontos);
}

void pedirNome(char nome[5]) {
    int posNome = 0;
    int offsetx = 6*3*posNome;

    tft.setTextSize(3);
    centralize(3, 120, 4);
    tft.print(nome);
    tft.fillRect(85+offsetx, 145, 5*3, 3, WHITE);

    while (1) {
        if (!digitalRead(CIMA_BUTTON)) {
            nome[posNome]++;
            if (nome[posNome] > 'Z') {
                nome[posNome] = 'A';
            }

            tft.drawChar(85+offsetx, 120, nome[posNome], WHITE, BLACK, 3);
        }

        if (!digitalRead(BAIXO_BUTTON)) {
            posNome++;
            if (posNome > 3) {
                posNome = 0;
            }
            offsetx = 6*3*posNome;

            tft.fillRect(85, 145, 69, 3, BLACK);
            tft.fillRect(85+offsetx, 145, 5*3, 3, WHITE);
        }

        if (!digitalRead(ENTER_BUTTON)) return;
        delay(150);
    }
}

void wait() {
    delay(300);
    while (1) {
        if (!digitalRead(ENTER_BUTTON)) return;
    }
}