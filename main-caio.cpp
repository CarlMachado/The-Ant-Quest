/*
* DESCRIÇÃO: Jogo criado para a disciplina de Algoritmos e Programação.
* AUTORES:
* Carlos Eduardo de Borba Machado
* Caio Gonzaga Bernils
* Mateus Luis Ropke Lauer
*/

// BIBLIOTECAS
//
#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <iostream>
#include <stdlib.h>
#include <ctype.h>
#include <list>
#include <time.h>
#include <conio.h>
#include <vector>
#include <string>
#include "pch.h"
//
using namespace std;

// CONSTANTES
//
#define LARGURA 960
#define ALTURA 740

#define TEMPO_MAXIMO 45.0

#define QUANTIDADE_ARMAZENS 3
#define QUANTIDADE_LOCAIS 4
//
// ENUMS
enum {
	PAREDE,
	CAMINHO,
	ITEM_PA,
	ITEM_BOTA,
	ITEM_TOCHA,
	ITEM_KIT,
	ARMAZEM_MAPA_1,
	ARMAZEM_MAPA_2,
	ARMAZEM_MAPA_3,
	FORMIGA_VAZIO,
	FORMIGA_CHEIO,
	BAD_ROCK
};
enum {
	ARMAZEM_1,
	ARMAZEM_2,
	ARMAZEM_3
};
enum {
	DESMORONANDO,
	NORMAL
};
enum {
	PEQUENA,
	GRANDE
};
enum {
	COMIDA_1,
	COMIDA_2,
	COMIDA_3,
	COMIDA_4,
	SEM_COMIDA
};
enum {
	LOCAL_1,
	LOCAL_2,
	LOCAL_3,
	LOCAL_4
};
enum {
	POSICAO_1,
	POSICAO_2,
	POSICAO_3,
	POSICAO_4
};
enum {
	SUBIR,
	DESCER,
	ESQUERDA,
	DIREITA,
	PEGAR,
	SAIR,
	NADA
};
enum {
	T10,
	T20,
	T30,
	T40,
	T50,
	T60,
	T70,
	T80,
	T90,
	T100,
	T110,
	T120,
	T130,
	T140,
	T150,
	TOTAL_FONTES
};
//

/*-------------------------------- STRUCTS ------------------------------------*/

typedef struct Controle {
	bool menu, jogar, pausa, sair, reiniciar;
	double tempoTotal, tempoExecucao;
	clock_t tempoInicial, tempoFinal;
	ALLEGRO_EVENT_QUEUE *fila_eventos;
	ALLEGRO_DISPLAY *display;
	ALLEGRO_BITMAP *imgPlaca;
	ALLEGRO_BITMAP *imgMenu;
	ALLEGRO_FONT *fonte[TOTAL_FONTES];
};

typedef struct Formiga {
	int comidaAtual;
	int x, y, DIRECAO;
	int tempoInicial, tempoDecorrido;
	int velocidade;
	bool vazio;
	ALLEGRO_BITMAP *imgFormiga[4];
};

typedef struct Mapa {
	int armazem[QUANTIDADE_ARMAZENS][QUANTIDADE_LOCAIS];
	int **mapa;
	int x, y;
	ALLEGRO_BITMAP *imgParede;
	ALLEGRO_BITMAP *imgCaminho;
	ALLEGRO_BITMAP *imgBackground;
	ALLEGRO_BITMAP *imgComida[4];
	ALLEGRO_BITMAP *imgSombra[2];
	ALLEGRO_BITMAP *imgComidaAmazem[4];
	ALLEGRO_BITMAP *imgArmazem[2];
};

typedef struct Item {
	int quantidadePa;
	bool tocha;
	double tempoTocha;
	double tempoFinalTocha;
	double tempoInicialTocha;
	bool bota;
	double tempoBota;
	double tempoFinalBota;
	double tempoInicialBota;
	ALLEGRO_BITMAP *imgPa;
	ALLEGRO_BITMAP *imgBota;
	ALLEGRO_BITMAP *imgTocha;
	ALLEGRO_BITMAP *imgKit;
};

/*-----------------------------------------------------------------------------*/




// PARTE DO CÓDIGO ESCRITA POR CAIO
//
// Inicia o tempo dos itens.
void tempoItem(bool inicio, Item &i) {
	if (inicio) {
		i.tempoInicialBota = clock();
		i.tempoInicialTocha = clock();
	}
	else {
		// Decorre o tempo para que os itens não se tornem eternos.
		i.tempoFinalBota = clock();
		i.tempoFinalTocha = clock();
		i.tempoBota -= (i.tempoFinalBota - i.tempoInicialBota) / (double)CLOCKS_PER_SEC;
		i.tempoTocha -= (i.tempoFinalTocha - i.tempoInicialTocha) / (double)CLOCKS_PER_SEC;
	}
}
// Sorteia a posição dos itens no Mapa.
void posicionarItens(Mapa &m, Item &i) {
	bool sortear = true;
	int x, y;
	srand(time(NULL));
	for (size_t ITEM = 2; ITEM < 6; ITEM++) {
		while (sortear) {
			x = rand() % m.x;
			y = rand() % m.y;
			if (m.mapa[y][x] == CAMINHO) {
				m.mapa[y][x] = ITEM;
				sortear = false;
			}
		}
		sortear = true;
	}
}
// Apresenta a tela de menu ao jogador
void menu(Controle &c) {
	int x = 180, y = 260;
	bool recordes, musica;
	int menuOp = 0;
	// Entra no Menu de Recordes.
	if (recordes) { 
		if (!al_is_event_queue_empty(c.fila_eventos)) {
			ALLEGRO_EVENT evento;
			al_wait_for_event(c.fila_eventos, &evento);
			if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
				switch (evento.keyboard.keycode)
				{
				// Caso as teclas Enter ou Espaço sejam pressionadas em Sair, retona ao Menu principal.
				case ALLEGRO_KEY_ENTER: 
				case ALLEGRO_KEY_SPACE:
					recordes = false;
					break;
				}
			}
			// Caso o jogo seja encerrado no botão X da Janela.
			else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) { 
				c.sair = true;
			}
		}
		// Posiciona as opções do Menu.
		al_draw_bitmap(c.imgMenu, 0, 0, NULL); 
		al_draw_text(c.fonte[T50], al_map_rgb(50, 25, 0), x + 140, y + 100, NULL, "INICIAR JOGO");
		al_draw_text(c.fonte[T50], al_map_rgb(50, 25, 0), x + 175, y + 200, NULL, "RECORDES");
		al_draw_text(c.fonte[T50], al_map_rgb(50, 25, 0), x + 200, y + 300, NULL, "MUSICA");
		al_draw_text(c.fonte[T50], al_map_rgb(50, 25, 0), x + 235, y + 400, NULL, "SAIR");

	}
	else if (musica) {
		if (!al_is_event_queue_empty(c.fila_eventos)) {
			ALLEGRO_EVENT evento;
			al_wait_for_event(c.fila_eventos, &evento);
			if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
				switch (evento.keyboard.keycode)
				{/*
				case ALLEGRO_KEY_W:
				case ALLEGRO_KEY_UP:
					menuOp--;
					if (menuOp < 0)
						menuOp = 0;
					break;
				case ALLEGRO_KEY_S:
				case ALLEGRO_KEY_DOWN:
					menuOp++;
					if (menuOp > 1)
						menuOp = 1;
					break;*/
				// Caso as teclas Enter ou Espaço sejam pressionadas em Sair, retona ao Menu principal.
				case ALLEGRO_KEY_ENTER: 
				case ALLEGRO_KEY_SPACE:
					musica = false;
					break;
				}
			}
			// Caso o jogo seja encerrado no botão X da Janela.
			else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) { 
				c.sair = true;
			}
		}
		// Posiciona as opções do Menu.
		al_draw_bitmap(c.imgMenu, 0, 0, NULL); 
		al_draw_text(c.fonte[T50], al_map_rgb(50, 25, 0), x + 140, y + 100, NULL, "INICIAR JOGO");
		al_draw_text(c.fonte[T50], al_map_rgb(50, 25, 0), x + 175, y + 200, NULL, "RECORDES");
		al_draw_text(c.fonte[T50], al_map_rgb(50, 25, 0), x + 200, y + 300, NULL, "MUSICA");
		al_draw_text(c.fonte[T50], al_map_rgb(50, 25, 0), x + 235, y + 400, NULL, "SAIR");
	}
	else {
		if (!al_is_event_queue_empty(c.fila_eventos)) {
			ALLEGRO_EVENT evento;
			al_wait_for_event(c.fila_eventos, &evento);
			if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
				switch (evento.keyboard.keycode)
				{
				// Caso a tecla W ou Seta para cima forem pressionadas, seleciona a opção de cima.
				case ALLEGRO_KEY_W: 
				case ALLEGRO_KEY_UP:
					menuOp--;
					if (menuOp < 0)
						menuOp = 0;
					break;
				// Caso a tecla S ou Seta para baixo forem pressionadas, seleciona a opção de baixo.
				case ALLEGRO_KEY_S: 
				case ALLEGRO_KEY_DOWN:
					menuOp++;
					if (menuOp > 0)
						menuOp = 0;
					break;
				case ALLEGRO_KEY_ENTER: 
				case ALLEGRO_KEY_SPACE:
					// Caso as teclas Enter ou Espaço forem pressionadas em Inciar, finaliza o Menu e inicia o jogo.
					if (menuOp == 0) { 
						c.menu = false;
						c.jogar = true;
					}
					// Caso as teclas Enter ou Espaço forem pressionadas em Recordes, entra no Menu de recordes.
					else if (menuOp == 1) { 
						recordes = true;
					}
					// Caso as teclas Enter ou Espaço forem pressionadas em Musica, entra no Menu de musicas.
					else if (menuOp == 2) { 
						musica = true;
					}
					// Caso as teclas Enter ou Espaço forem pressionadas em Sair, finaliza o jogo.
					else if (menuOp == 3) { 
						c.sair = true;
					}
					break;
				}
			}
			// Caso o jogo seja encerrado no botão X da Janela.
			else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) { 
				c.sair = true;
			}
		}
		// Posiciona as opções do Menu.
		al_draw_bitmap(c.imgMenu, 0, 0, NULL); 
		al_draw_text(c.fonte[T50], al_map_rgb(50, 25, 0), x + 140, y + 100, NULL, "INICIAR JOGO");
		al_draw_text(c.fonte[T50], al_map_rgb(50, 25, 0), x + 175, y + 200, NULL, "RECORDES");
		al_draw_text(c.fonte[T50], al_map_rgb(50, 25, 0), x + 200, y + 300, NULL, "MUSICA");
		al_draw_text(c.fonte[T50], al_map_rgb(50, 25, 0), x + 235, y + 400, NULL, "SAIR");
	}

}
//
// FIM DA PARTE DE CÓDIGO ESCRITA POR CAIO



/*---------------------------------------------------------------------------------------------------*/



// PARTE DO CÓDIGO ESCRITA POR CARLOS
//
// Verifica se a formiga está ou não com comida.
void formigaAtual(Mapa &m, Formiga f) {
	if (f.vazio)
		m.mapa[f.y][f.x] = FORMIGA_VAZIO;
	else
		m.mapa[f.y][f.x] = FORMIGA_CHEIO;
}

// Inicializa os armazens.
void iniciarArmazem(int armazem[QUANTIDADE_ARMAZENS][QUANTIDADE_LOCAIS]) {
	int COMIDA = 3;
	for (size_t ARMAZEM = 0; ARMAZEM < QUANTIDADE_ARMAZENS; ARMAZEM++) {
		for (size_t LOCAL = 0; LOCAL < QUANTIDADE_LOCAIS; LOCAL++) {
			if (ARMAZEM == ARMAZEM_1) {
				armazem[ARMAZEM_1][LOCAL] = COMIDA;
				COMIDA--;
			}
			else {
				armazem[ARMAZEM][LOCAL] = SEM_COMIDA;
			}
		}
	}
}

// Gera um novo mapa.
void novoMapa(Mapa &m) {
	list <pair <int, int> > drillers;
	int **map = NULL;
	int cont = 0;
	bool novoMapa = true;

	srand(time(NULL));

	// Verifica se o tamanho da matriz é par para encaixar na textura na hr da impressão.
	m.x = 15 + (rand() % 8);
	if (m.x % 2 != 0)
		m.x++;
	m.y = (m.x / 3) * 2;
	if (m.y % 2 != 0)
		m.y++;

	// Loop para criar o mapa, onde ele fica recriando até que saia um mapa com uma quantidade desejável de corredores.
	while (novoMapa) {
		cont = 0;
		map = new int *[m.y];

		for (size_t y = 0; y < m.y; y++)
			map[y] = new int[m.x];

		for (size_t x = 0; x < m.x; x++)
			for (size_t y = 0; y < m.y; y++)
				map[y][x] = PAREDE;

		drillers.push_back(make_pair(m.x / 2, m.y / 2));

		while (drillers.size() > 0) {
			list <pair <int, int> >::iterator n, _n, temp;
			n = drillers.begin();
			_n = drillers.end();
			while (n != _n) {
				bool remove_driller = false;
				switch (rand() % 4)
				{
				case 0:
					(*n).second -= 2;
					if ((*n).second < 0 || map[(*n).second][(*n).first]) {
						remove_driller = true;
						break;
					}
					map[(*n).second + 1][(*n).first] = CAMINHO;
					break;
				case 1:
					(*n).second += 2;
					if ((*n).second >= m.y || map[(*n).second][(*n).first]) {
						remove_driller = true;
						break;
					}
					map[(*n).second - 1][(*n).first] = CAMINHO;
					break;
				case 2:
					(*n).first -= 2;
					if ((*n).first < 0 || map[(*n).second][(*n).first]) {
						remove_driller = true;
						break;
					}
					map[(*n).second][(*n).first + 1] = CAMINHO;
					break;
				case 3:
					(*n).first += 2;
					if ((*n).first >= m.x || map[(*n).second][(*n).first]) {
						remove_driller = true;
						break;
					}
					map[(*n).second][(*n).first - 1] = CAMINHO;
					break;
				}
				if (remove_driller) {
					n = drillers.erase(n);
				}
				else {
					drillers.push_back(make_pair((*n).first, (*n).second));
					if (rand() % 2)
						drillers.push_back(make_pair((*n).first, (*n).second));
					map[(*n).second][(*n).first] = CAMINHO;
					++n;
				}
			}
		}

		// Realiza contagem da quantidade de CAMINHOS para que não gere um mapa muito pequeno.
		for (size_t x = 0; x < m.x; x++)
			for (size_t y = 0; y < m.y; y++)
				if (map[y][x] == CAMINHO)
					cont++;
		if (cont > 75)
			novoMapa = false;
		else
			delete map;

	}

	// Coloca uma matriz dentro da outra para que tenha sempre uma "muralha em volta".
	m.x += 2;
	m.y += 2;

	m.mapa = new int *[m.y];

	for (size_t y = 0; y < m.y; y++)
		m.mapa[y] = new int[m.x];

	for (size_t x = 0; x < m.x; x++)
		for (size_t y = 0; y < m.y; y++)
			m.mapa[y][x] = BAD_ROCK;

	for (size_t x = 1; x < m.x - 1; x++)
		for (size_t y = 1; y < m.y - 1; y++)
			m.mapa[y][x] = map[y - 1][x - 1];

	delete map;
}

// Posiciona uma nova formiga no mapa.
void posicionarFormiga(Mapa &m, Formiga &f) {
	bool sortear = true;
	srand(time(NULL));
	while (sortear) {
		f.x = rand() % m.x;
		f.y = rand() % m.y;
		if (m.mapa[f.y][f.x] == CAMINHO) {
			if (f.vazio)
				m.mapa[f.y][f.x] = FORMIGA_VAZIO;
			else
				m.mapa[f.y][f.x] = FORMIGA_CHEIO;
			sortear = false;
		}
	}
}

// Função que verifica se vai retirar ou colocar a comida no armazém.
void verificarArmazem(Formiga &f, Mapa &m, int ARMAZEM) {
	int LOCAL;
	bool retirar = false;
	// Se a formiga não tiver segurando comida, significa que ela vai pegar do armazém.
	if (f.comidaAtual == 0) { 
		for (LOCAL = 3; LOCAL >= 0; LOCAL--) {
			if (m.armazem[ARMAZEM][LOCAL] != SEM_COMIDA) {
				f.comidaAtual = m.armazem[ARMAZEM][LOCAL];
				m.armazem[ARMAZEM][LOCAL] = SEM_COMIDA;
				m.mapa[f.x][f.y] = FORMIGA_CHEIO;
				f.vazio = false;
				LOCAL = -1;
			}
		}
	}
	// Se já possuir, ela vai depositar no armazém.
	else { 
		for (LOCAL = 0; LOCAL <= 3; LOCAL++) {
			if (m.armazem[ARMAZEM][LOCAL] == SEM_COMIDA) {
				if (LOCAL == 0)
					retirar = true;
				else if (f.comidaAtual < m.armazem[ARMAZEM][LOCAL - 1])
					retirar = true;

				if (retirar) {
					m.armazem[ARMAZEM][LOCAL] = f.comidaAtual;
					f.comidaAtual = SEM_COMIDA;
					m.mapa[f.x][f.y] = FORMIGA_VAZIO;
					f.vazio = true;
				}
				LOCAL = 4;
			}
		}
	}
}

// Nessa função são executados os comandos do jogo.
// Obs.: Todos os comentários dessa função servem para todas as posições pressionadas, apenas para redução de linhas.
void lerComandos(Mapa &m, Controle &c, Formiga &f, Item &i) {
	f.tempoDecorrido = clock();
	if (!al_is_event_queue_empty(c.fila_eventos)) {
		ALLEGRO_EVENT evento;
		al_wait_for_event(c.fila_eventos, &evento);
		if (evento.type == ALLEGRO_EVENT_KEY_CHAR) {
			switch (evento.keyboard.keycode)
			{
			case ALLEGRO_KEY_W:
			case ALLEGRO_KEY_UP:
				if (f.tempoDecorrido - f.tempoInicial > f.velocidade) {
					if (m.mapa[f.y - 1][f.x] == CAMINHO ||
						m.mapa[f.y - 1][f.x] == ITEM_PA ||
						m.mapa[f.y - 1][f.x] == ITEM_BOTA ||
						m.mapa[f.y - 1][f.x] == ITEM_TOCHA ||
						m.mapa[f.y - 1][f.x] == ITEM_KIT) {
						// Verifica se nessa posição existe uma PÁ.
						if (m.mapa[f.y - 1][f.x] == ITEM_PA) {
							// Ao pegar o item PÁ, acrescenta mais um no contador.
							i.quantidadePa++;
						}
						// Verifica se nessa posição existe uma BOTA.
						if (m.mapa[f.y - 1][f.x] == ITEM_BOTA) {
						// Caso ele já obtiver a BOTA, acrescenta 30 segundos.
							if (i.bota) {
								i.tempoBota += 30.0;
							}
						// Ao pegar o item BOTA, inicia a função do item.
							else {
								i.bota = true;
								i.tempoBota = 30.0;
							}
						}
						// Verifica se nessa posição existe uma TOCHA.
						if (m.mapa[f.y - 1][f.x] == ITEM_TOCHA) {
							// Caso ele já obtiver a TOCHA, acrescenta 30 segundos.
							if (i.tocha) {
								i.tempoTocha += 30.0;
							}
							// Ao pegar o item TOCHA, inicia a função do item.
							else {
								i.tocha = true;
								i.tempoTocha = 30.0;
							}
						}
						// Verifica se nessa posição existe um KIT DE REPAROS.
						if (m.mapa[f.y - 1][f.x] == ITEM_KIT) {
							// Inicia a função do item, acrescenta 30 segundos no tempo até o próximo terremoto.
							c.tempoTotal += 30;
						}
						m.mapa[f.y][f.x] = CAMINHO;
						f.y--;
						formigaAtual(m, f);
					}
					// Contabiliza o tempo total do jogo.
					f.tempoInicial = f.tempoDecorrido;
				}
				f.DIRECAO = SUBIR;
				break;
			case ALLEGRO_KEY_S:
			case ALLEGRO_KEY_DOWN:
				if (f.tempoDecorrido - f.tempoInicial > f.velocidade) {
					if (m.mapa[f.y + 1][f.x] == CAMINHO ||
						m.mapa[f.y + 1][f.x] == ITEM_PA ||
						m.mapa[f.y + 1][f.x] == ITEM_BOTA ||
						m.mapa[f.y + 1][f.x] == ITEM_TOCHA ||
						m.mapa[f.y + 1][f.x] == ITEM_KIT) {
						if (m.mapa[f.y + 1][f.x] == ITEM_PA) {
							i.quantidadePa++;
						}
						if (m.mapa[f.y + 1][f.x] == ITEM_BOTA) {
							if (i.bota) {
								i.tempoBota += 30.0;
							}
							else {
								i.bota = true;
								i.tempoBota = 30.0;
							}
						}
						if (m.mapa[f.y + 1][f.x] == ITEM_TOCHA) {
							if (i.tocha) {
								i.tempoTocha += 30.0;
							}
							else {
								i.tocha = true;
								i.tempoTocha = 30.0;
							}
						}
						if (m.mapa[f.y + 1][f.x] == ITEM_KIT) {
							c.tempoTotal += 30;
						}
						m.mapa[f.y][f.x] = CAMINHO;
						f.y++;
						formigaAtual(m, f);
					}
					f.tempoInicial = f.tempoDecorrido;
				}
				f.DIRECAO = DESCER;
				break;
			case ALLEGRO_KEY_A:
			case ALLEGRO_KEY_LEFT:
				if (f.tempoDecorrido - f.tempoInicial > f.velocidade) {
					if (m.mapa[f.y][f.x - 1] == CAMINHO ||
						m.mapa[f.y][f.x - 1] == ITEM_PA ||
						m.mapa[f.y][f.x - 1] == ITEM_BOTA ||
						m.mapa[f.y][f.x - 1] == ITEM_TOCHA ||
						m.mapa[f.y][f.x - 1] == ITEM_KIT) {
						if (m.mapa[f.y][f.x - 1] == ITEM_PA) {
							i.quantidadePa++;
						}
						if (m.mapa[f.y][f.x - 1] == ITEM_BOTA) {
							if (i.bota) {
								i.tempoBota += 30.0;
							}
							else {
								i.bota = true;
								i.tempoBota = 30.0;
							}
						}
						if (m.mapa[f.y][f.x - 1] == ITEM_TOCHA) {
							if (i.tocha) {
								i.tempoTocha += 30.0;
							}
							else {
								i.tocha = true;
								i.tempoTocha = 30.0;
							}
						}
						if (m.mapa[f.y][f.x - 1] == ITEM_KIT) {
							c.tempoTotal += 30;
						}
						m.mapa[f.y][f.x] = CAMINHO;
						f.x--;
						formigaAtual(m, f);
					}
					f.tempoInicial = f.tempoDecorrido;
				}
				f.DIRECAO = ESQUERDA;
				break;
			case ALLEGRO_KEY_D:
			case ALLEGRO_KEY_RIGHT:
				if (f.tempoDecorrido - f.tempoInicial > f.velocidade) {
					if (m.mapa[f.y][f.x + 1] == CAMINHO ||
						m.mapa[f.y][f.x + 1] == ITEM_PA ||
						m.mapa[f.y][f.x + 1] == ITEM_BOTA ||
						m.mapa[f.y][f.x + 1] == ITEM_TOCHA ||
						m.mapa[f.y][f.x + 1] == ITEM_KIT) {
						if (m.mapa[f.y][f.x + 1] == ITEM_PA) {
							i.quantidadePa++;
						}
						if (m.mapa[f.y][f.x + 1] == ITEM_BOTA) {
							if (i.bota) {
								i.tempoBota += 30.0;
							}
							else {
								i.bota = true;
								i.tempoBota = 30.0;
							}
						}
						if (m.mapa[f.y][f.x + 1] == ITEM_TOCHA) {
							if (i.tocha) {
								i.tempoTocha += 30.0;
							}
							else {
								i.tocha = true;
								i.tempoTocha = 30.0;
							}
						}
						if (m.mapa[f.y][f.x + 1] == ITEM_KIT) {
							c.tempoTotal += 30;
						}
						m.mapa[f.y][f.x] = CAMINHO;
						f.x++;
						formigaAtual(m, f);
					}
					f.tempoInicial = f.tempoDecorrido;
				}
				f.DIRECAO = DIREITA;
				break;
			case ALLEGRO_KEY_ESCAPE:
				c.pausa = true;
				c.jogar = false;
				break;
			case ALLEGRO_KEY_ENTER:
			case ALLEGRO_KEY_SPACE:
				// Pega ou deposita comida.
				// Se alguma posição ao redor da formiga for armazém ela chama a função verificar armazém.
				if (m.mapa[f.y + 1][f.x] == ARMAZEM_MAPA_1 ||
					m.mapa[f.y - 1][f.x] == ARMAZEM_MAPA_1 ||
					m.mapa[f.y][f.x + 1] == ARMAZEM_MAPA_1 ||
					m.mapa[f.y][f.x - 1] == ARMAZEM_MAPA_1)		// O ARMAZÉM 1 É O INICIAL (DESMORONANDO)
					verificarArmazem(f, m, ARMAZEM_1);
				if (m.mapa[f.y + 1][f.x] == ARMAZEM_MAPA_2 ||
					m.mapa[f.y - 1][f.x] == ARMAZEM_MAPA_2 ||
					m.mapa[f.y][f.x + 1] == ARMAZEM_MAPA_2 ||
					m.mapa[f.y][f.x - 1] == ARMAZEM_MAPA_2)
					verificarArmazem(f, m, ARMAZEM_2);
				if (m.mapa[f.y + 1][f.x] == ARMAZEM_MAPA_3 ||
					m.mapa[f.y - 1][f.x] == ARMAZEM_MAPA_3 ||
					m.mapa[f.y][f.x + 1] == ARMAZEM_MAPA_3 ||
					m.mapa[f.y][f.x - 1] == ARMAZEM_MAPA_3)
					verificarArmazem(f, m, ARMAZEM_3);
				// Verifica se a frente da formiga existe parede, se sim, utiliza o item PÁ destruindo a parede selecionada.
				if (m.mapa[f.y - 1][f.x] == PAREDE && f.DIRECAO == SUBIR && i.quantidadePa > 0) {
					m.mapa[f.y - 1][f.x] = CAMINHO;
					// Diminui a quantidade de PÁS restantes.
					i.quantidadePa--;
				}
				if (m.mapa[f.y + 1][f.x] == PAREDE && f.DIRECAO == DESCER && i.quantidadePa > 0) {
					m.mapa[f.y + 1][f.x] = CAMINHO;
					i.quantidadePa--;
				}
				if (m.mapa[f.y][f.x + 1] == PAREDE && f.DIRECAO == DIREITA && i.quantidadePa > 0) {
					m.mapa[f.y][f.x + 1] = CAMINHO;
					i.quantidadePa--;
				}
				if (m.mapa[f.y][f.x - 1] == PAREDE && f.DIRECAO == ESQUERDA && i.quantidadePa > 0) {
					m.mapa[f.y][f.x - 1] = CAMINHO;
					i.quantidadePa--;
				}
			}
		}
		else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			c.sair = true;
		}
		// Inicia a função do item BOTA, aumentando a velocidade máxima.
		if (i.bota) {
			f.velocidade = 100;
		}
		// Velocidade padrão.
		else {
			f.velocidade = 200;
		}
		// Desabilita o efeito do item BOTA após o término do tempo.
		if (i.tempoBota < 0.0) {
			i.bota = false;
		}
		// Desabilita o efeito do item TOCHA após o término do tempo.
		if (i.tempoTocha < 0.0) {
			i.tocha = false;
		}
	}
}

// Nessa função a matriz é percorrida e os números são substituidos
void desenharFrame(Mapa m, Controle c, Formiga f, Item it) {
	const int TILE = 40;
	const int PLACAR = 100;
	string tempoString = to_string((int)c.tempoTotal);
	char const* tempoChar = tempoString.c_str();
	string tempoStringTocha = to_string((int)it.tempoTocha);
	char const* tempoTocha = tempoStringTocha.c_str();
	string tempoStringBota = to_string((int)it.tempoBota);
	char const* tempoBota = tempoStringBota.c_str();
	string paString = to_string((int)it.quantidadePa);
	char const* qtdPa = paString.c_str();
	int x = (LARGURA / 2) - ((m.x * TILE) / 2), y = (PLACAR / 2) + (ALTURA / 2) - ((m.y * TILE) / 2);
	int sombraX, sombraY;
	al_draw_bitmap(m.imgBackground, 0, PLACAR, NULL);
	for (size_t i = 0; i < m.y; i++) {
		for (size_t j = 0; j < m.x; j++) {
			if (m.mapa[i][j] == PAREDE || m.mapa[i][j] == BAD_ROCK) {
				al_draw_bitmap(m.imgCaminho, x, y, NULL);
				al_draw_bitmap(m.imgParede, x, y, NULL);
			}
			// Renderiza a imagem do Caminho.
			else if (m.mapa[i][j] == CAMINHO) {
				al_draw_bitmap(m.imgCaminho, x, y, NULL);
				al_draw_bitmap(m.imgCaminho, x, y, NULL);
			}
			// Renderiza a imagem do Item Pá.
			else if (m.mapa[i][j] == ITEM_PA) {
				al_draw_bitmap(m.imgCaminho, x, y, NULL);
				al_draw_bitmap(it.imgPa, x, y, NULL);
			}
			// Renderiza a imagem do Item Bota.
			else if (m.mapa[i][j] == ITEM_BOTA) {
				al_draw_bitmap(m.imgCaminho, x, y, NULL);
				al_draw_bitmap(it.imgBota, x, y, NULL);
			}
			// Renderiza a imagem do Item Tocha.
			else if (m.mapa[i][j] == ITEM_TOCHA) {
				al_draw_bitmap(m.imgCaminho, x, y, NULL);
				al_draw_bitmap(it.imgTocha, x, y, NULL);
			}
			// Renderiza a imagem do Item Kit de Reparos.
			else if (m.mapa[i][j] == ITEM_KIT) {
				al_draw_bitmap(m.imgCaminho, x, y, NULL);
				al_draw_bitmap(it.imgKit, x, y, NULL);
			}
			// Renderiza a imagem dos Armazens.
			else if (m.mapa[i][j] == ARMAZEM_MAPA_1) {
				al_draw_bitmap(m.imgCaminho, x, y, NULL);
				al_draw_bitmap(m.imgArmazem[DESMORONANDO], x, y, NULL);
				// Renderiza a pilha de comidas dentro do armazém.
				if (m.armazem[ARMAZEM_1][LOCAL_1])
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_4], x, y, NULL);
				if (m.armazem[ARMAZEM_1][LOCAL_2])
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_3], x, y, NULL);
				if (m.armazem[ARMAZEM_1][LOCAL_3])
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_2], x, y, NULL);
				if (m.armazem[ARMAZEM_1][LOCAL_4])
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_1], x, y, NULL);
			}
			else if (m.mapa[i][j] == ARMAZEM_MAPA_2) {
				al_draw_bitmap(m.imgCaminho, x, y, NULL);
				al_draw_bitmap(m.imgArmazem[NORMAL], x, y, NULL);
				if (m.armazem[ARMAZEM_2][LOCAL_1])
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_4], x, y, NULL);
				if (m.armazem[ARMAZEM_2][LOCAL_2])
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_3], x, y, NULL);
				if (m.armazem[ARMAZEM_2][LOCAL_3])
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_2], x, y, NULL);
				if (m.armazem[ARMAZEM_2][LOCAL_4])
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_1], x, y, NULL);
			}
			else if (m.mapa[i][j] == ARMAZEM_MAPA_3) {
				al_draw_bitmap(m.imgCaminho, x, y, NULL);
				al_draw_bitmap(m.imgArmazem[NORMAL], x, y, NULL);
				if (m.armazem[ARMAZEM_3][LOCAL_1])
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_4], x, y, NULL);
				if (m.armazem[ARMAZEM_3][LOCAL_2])
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_3], x, y, NULL);
				if (m.armazem[ARMAZEM_3][LOCAL_3])
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_2], x, y, NULL);
				if (m.armazem[ARMAZEM_3][LOCAL_4])
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_1], x, y, NULL);
			}
			// Renderiza a formiga sem comida.
			else if (m.mapa[i][j] == FORMIGA_VAZIO) {
				sombraX = x + TILE / 2 - LARGURA;
				sombraY = y + TILE / 2 + PLACAR - ALTURA;
				al_draw_bitmap(m.imgCaminho, x, y, NULL);
				al_draw_bitmap(f.imgFormiga[f.DIRECAO], x, y, NULL);
			}
			// Renderiza a formiga com comida.
			else if (m.mapa[i][j] == FORMIGA_CHEIO) {
				sombraX = x + TILE / 2 - LARGURA;
				sombraY = y + TILE / 2 + PLACAR - ALTURA;
				al_draw_bitmap(m.imgCaminho, x, y, NULL);
				al_draw_bitmap(f.imgFormiga[f.DIRECAO], x, y, NULL);
				al_draw_bitmap(m.imgComida[f.DIRECAO], x, y, NULL);
			}
			x += TILE;
		}
		y += TILE;
		x = (LARGURA / 2) - ((m.x * TILE) / 2);
	}
	// Aumenta o tamanho da visão da formiga, efeito do item TOCHA.
	if (it.tocha)
		al_draw_bitmap(m.imgSombra[GRANDE], sombraX, sombraY, NULL);
	// Renderiza o efeito que impede a visão da formiga.
	else
		al_draw_bitmap(m.imgSombra[PEQUENA], sombraX, sombraY, NULL);
	/*---------------------------- HUD -------------------------------*/
	al_draw_bitmap(c.imgPlaca, 0, 0, NULL);
	al_draw_text(c.fonte[T10], al_map_rgb(102, 51, 0), 490, 15, NULL, "tempo ate o terremoto: ");
	al_draw_text(c.fonte[T10], al_map_rgb(102, 51, 0), 670, 15, NULL, tempoChar);
	al_draw_text(c.fonte[T10], al_map_rgb(102, 51, 0), 490, 30, NULL, "bota: ");
	if (it.bota)
		al_draw_text(c.fonte[T10], al_map_rgb(102, 51, 0), 670, 30, NULL, tempoBota);
	else
		al_draw_text(c.fonte[T10], al_map_rgb(102, 51, 0), 670, 30, NULL, "nao possui");
	al_draw_text(c.fonte[T10], al_map_rgb(102, 51, 0), 490, 45, NULL, "tocha: ");
	if (it.tocha)
		al_draw_text(c.fonte[T10], al_map_rgb(102, 51, 0), 670, 45, NULL, tempoTocha);
	else
		al_draw_text(c.fonte[T10], al_map_rgb(102, 51, 0), 670, 45, NULL, "nao possui");
	al_draw_text(c.fonte[T10], al_map_rgb(102, 51, 0), 490, 60, NULL, "pa: ");
	al_draw_text(c.fonte[T10], al_map_rgb(102, 51, 0), 670, 60, NULL, qtdPa);
	/*----------------------------------------------------------------*/
}

// Carrega o modo gráfico
void inicializarAllegro(Controle &c) {
	al_init();
	al_set_new_display_flags(ALLEGRO_OPENGL);
	c.display = al_create_display(LARGURA, ALTURA);
	al_set_window_title(c.display, "The Ant Quest");
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_install_keyboard();
	c.fila_eventos = al_create_event_queue();
	al_register_event_source(c.fila_eventos, al_get_keyboard_event_source());
	al_register_event_source(c.fila_eventos, al_get_display_event_source(c.display));
}

//Carrega os recursos do jogo.
void carregarRecursos(Mapa &m, Controle &c, Formiga &f, Item &i) {
	int tamCont = 10;
	c.imgPlaca = al_load_bitmap("res/PLACA.png");
	c.imgMenu = al_load_bitmap("res/MENU.png");
	for (size_t TAM = 0; TAM < TOTAL_FONTES; TAM++) {
		c.fonte[TAM] = al_load_font("res/COMIC.ttf", tamCont, NULL);
		tamCont += 10;
	}
	m.imgParede = al_load_bitmap("res/PAREDE.png");
	m.imgCaminho = al_load_bitmap("res/CAMINHO.png");
	m.imgBackground = al_load_bitmap("res/BACKGROUND.png");
	m.imgComida[SUBIR] = al_load_bitmap("res/COMIDA_CIMA.png");
	m.imgComida[DESCER] = al_load_bitmap("res/COMIDA_BAIXO.png");
	m.imgComida[ESQUERDA] = al_load_bitmap("res/COMIDA_ESQUERDA.png");
	m.imgComida[DIREITA] = al_load_bitmap("res/COMIDA_DIREITA.png");
	m.imgSombra[GRANDE] = al_load_bitmap("res/SOMBRA_GRANDE.png");
	m.imgSombra[PEQUENA] = al_load_bitmap("res/SOMBRA_PEQUENA.png");
	m.imgArmazem[DESMORONANDO] = al_load_bitmap("res/ARMAZEM_DESMORONANDO.png");
	m.imgArmazem[NORMAL] = al_load_bitmap("res/ARMAZEM.png");
	m.imgComidaAmazem[COMIDA_1] = al_load_bitmap("res/COMIDA_1.png");
	m.imgComidaAmazem[COMIDA_2] = al_load_bitmap("res/COMIDA_2.png");
	m.imgComidaAmazem[COMIDA_3] = al_load_bitmap("res/COMIDA_3.png");
	m.imgComidaAmazem[COMIDA_4] = al_load_bitmap("res/COMIDA_4.png");
	f.imgFormiga[SUBIR] = al_load_bitmap("res/FORMIGA_CIMA.png");
	f.imgFormiga[DESCER] = al_load_bitmap("res/FORMIGA_BAIXO.png");
	f.imgFormiga[ESQUERDA] = al_load_bitmap("res/FORMIGA_ESQUERDA.png");
	f.imgFormiga[DIREITA] = al_load_bitmap("res/FORMIGA_DIREITA.png");
	i.imgBota = al_load_bitmap("res/ITEM_BOTA.png");
	i.imgKit = al_load_bitmap("res/ITEM_KIT.png");
	i.imgPa = al_load_bitmap("res/ITEM_PA.png");
	i.imgTocha = al_load_bitmap("res/ITEM_TOCHA.png");
}

// Verifica se o jogador venceu.
bool venceu(int a[QUANTIDADE_ARMAZENS][QUANTIDADE_LOCAIS]) {
	int cont = 0;
	for (size_t LOCAL = 0; LOCAL < QUANTIDADE_LOCAIS; LOCAL++)
		if (a[ARMAZEM_1][LOCAL] == SEM_COMIDA)
			cont++;
	if (cont == 4)
		return true;
	else
		return false;
}

// Tela de fim de jogo.
void fimJogo(Controle &c) {
	int x = 180, y = 260;
	string tempoString = to_string((int)c.tempoExecucao);
	char const* tempoChar = tempoString.c_str();
	al_draw_bitmap(c.imgMenu, 0, 0, NULL);
	al_draw_text(c.fonte[T50], al_map_rgb(50, 25, 0), x + 140, y + 100, NULL, "parabens!");
	al_draw_text(c.fonte[T50], al_map_rgb(50, 25, 0), x + 0, y + 200, NULL, "seu tempo foi de: ");
	al_draw_text(c.fonte[T50], al_map_rgb(50, 25, 0), x + 530, y + 200, NULL, tempoChar);
	al_draw_text(c.fonte[T20], al_map_rgb(50, 25, 0), x + 90, y + 400, NULL, "aperte ENTER para continuar");
	c.jogar = false;
	if (!al_is_event_queue_empty(c.fila_eventos)) {
		ALLEGRO_EVENT evento;
		al_wait_for_event(c.fila_eventos, &evento);
		if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch (evento.keyboard.keycode)
			{
			case ALLEGRO_KEY_ENTER:
				c.reiniciar = true;
			}
		}
		else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			c.sair = true;
		}
	}
}
// Inicializa todas as variaveis do programa e atribui seus valores.
void inicializarVariaveis(Mapa &m, Controle &c, Formiga &f, Item &i) {
	c.menu = true;
	c.jogar = false;
	c.pausa = false;
	c.sair = false;
	c.reiniciar = false;
	c.tempoInicial = 0;
	c.tempoFinal = 0;
	c.tempoTotal = TEMPO_MAXIMO;
	c.tempoExecucao = 0;
	f.comidaAtual = SEM_COMIDA;
	f.DIRECAO = SUBIR;
	f.velocidade = 200;
	f.vazio = true;
	i.tempoBota = 0.0;
	i.tempoFinalBota = 0.0;
	i.tempoFinalTocha = 0.0;
	i.tempoInicialBota = 0.0;
	i.tempoInicialTocha = 0.0;
	i.tempoTocha = 0.0;
	i.quantidadePa = 0;
	i.tocha = false;
	i.bota = false;
}

// Finaliza o programa.
void finalizar(Mapa &m, Controle &c, Formiga &f, Item &i) {
	al_destroy_display(c.display);
	al_destroy_event_queue(c.fila_eventos);
	for (size_t TAM = 0; TAM < TOTAL_FONTES; TAM++)
		al_destroy_font(c.fonte[TAM]);
	al_destroy_bitmap(c.imgPlaca);
	al_destroy_bitmap(c.imgMenu);
	al_destroy_bitmap(m.imgParede);
	al_destroy_bitmap(m.imgCaminho);
	al_destroy_bitmap(m.imgBackground);
	al_destroy_bitmap(m.imgComida[SUBIR]);
	al_destroy_bitmap(m.imgComida[DESCER]);
	al_destroy_bitmap(m.imgComida[ESQUERDA]);
	al_destroy_bitmap(m.imgComida[DIREITA]);
	al_destroy_bitmap(m.imgSombra[GRANDE]);
	al_destroy_bitmap(m.imgSombra[PEQUENA]);
	al_destroy_bitmap(m.imgArmazem[DESMORONANDO]);
	al_destroy_bitmap(m.imgArmazem[NORMAL]);
	al_destroy_bitmap(m.imgComidaAmazem[COMIDA_1]);
	al_destroy_bitmap(m.imgComidaAmazem[COMIDA_2]);
	al_destroy_bitmap(m.imgComidaAmazem[COMIDA_3]);
	al_destroy_bitmap(m.imgComidaAmazem[COMIDA_4]);
	al_destroy_bitmap(f.imgFormiga[SUBIR]);
	al_destroy_bitmap(f.imgFormiga[DESCER]);
	al_destroy_bitmap(f.imgFormiga[ESQUERDA]);
	al_destroy_bitmap(f.imgFormiga[DIREITA]);
	al_destroy_bitmap(i.imgBota);
	al_destroy_bitmap(i.imgKit);
	al_destroy_bitmap(i.imgPa);
	al_destroy_bitmap(i.imgTocha);
}

// Reinicia as variáveis.
void inicializarJogo(Mapa &m, Controle &c, Formiga &f, Item &i) {
	inicializarVariaveis(m, c, f, i);
	novoMapa(m);
	posicionarFormiga(m, f);
	iniciarArmazem(m.armazem);
	posicionarItens(m, i);
}
//
// FIM DA PARTE DE CÓDIGO ESCRITA POR CARLOS



/*---------------------------------------------------------------------------------------------------*/



// PARTE DO CÓDIGO ESCRITA POR MATEUS
//
// função para medir tempo e sortear novo mapa.
void medirTempo(bool inicio, Mapa &m, Controle &c, Formiga &f, Item &i) {
	if (inicio) {
		c.tempoInicial = clock();
	}
	else {
		c.tempoFinal = clock();
		c.tempoTotal -= (c.tempoFinal - c.tempoInicial) / (double)CLOCKS_PER_SEC;
		c.tempoExecucao += (c.tempoFinal - c.tempoInicial) / (double)CLOCKS_PER_SEC;

		if (c.tempoTotal < 0) {
			novoMapa(m);
			posicionarFormiga(m, f);
			// FALTA CRIAR A FUNÇÃO QUE DISPÕE OS ARMAZÉNS NO MAPA (MATEUS)
			posicionarItens(m, i);
			c.tempoTotal = TEMPO_MAXIMO;
		}
	}
}
void pausa(Controle c) {

}
//
// FIM DA PARTE DE CÓDIGO ESCRITA POR MATEUS



/*---------------------------------------------------------------------------------------------------*/



// TANTO A FUNÇÃO MAIN COMO SUAS VARIÁVEIS FORAM MANIPULADAS POR TODO O GRUPO
//
int main(void) {
	/*-------------------------------- VARIÁVEIS ------------------------------*/

	Mapa m;
	Formiga f;
	Controle c;
	Item i;

	/*-------------------------------------------------------------------------*/

	/*------------------------------ INICIALIZAÇÃO ----------------------------*/

	inicializarAllegro(c);
	carregarRecursos(m, c, f, i);
	inicializarJogo(m, c, f, i);

	/*------------------------------ LOOP PRINCIPAL ---------------------------*/

	while (!c.sair) {
		al_clear_to_color(al_map_rgb(0, 0, 0));
		// Inicia o menu do jogo, com 4 opções.
		if (c.menu) {
			menu(c);
		}
		// Inicia o jogo, chamando todas as funções.
		else if (c.jogar) {
			tempoItem(true, i);
			medirTempo(true, m, c, f, i);
			desenharFrame(m, c, f, i);
			lerComandos(m, c, f, i);
			medirTempo(false, m, c, f, i);
			tempoItem(false, i);
		}
		// Ativa o menu de pausa.
		else if (c.pausa) {
			pausa(c);
		}
		// Finaliza o jogo após vencer.
		if (venceu(m.armazem) && f.vazio) {
			fimJogo(c);
			if (c.reiniciar)
				inicializarJogo(m, c, f, i);
		}
		al_flip_display();
	}

	/*-------------------------------------------------------------------------*/

	finalizar(m, c, f, i);

	return 0;
}
//
