/*
* DESCRIÇÃO: Jogo criado para a disciplina de Algoritmos e Programação.
* AUTORES:
* Carlos Eduardo de Borba Machado
* Caio Gonzaga Bernils
* Mateus Luis Ropke Lauer
*/

// BIBLIOTECAS
//
#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_acodec.h>
#include<allegro5/allegro_audio.h>
#include <allegro5/allegro_ttf.h>
#include <time.h>
#include <list>
#include <string>
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
	PAREDE, CAMINHO, ITEM_PA, ITEM_BOTA, ITEM_TOCHA, ITEM_KIT, ARMAZEM_MAPA_1, ARMAZEM_MAPA_2, ARMAZEM_MAPA_3, FORMIGA_VAZIO, FORMIGA_CHEIO, BAD_ROCK
};
enum {
	ARMAZEM_1, ARMAZEM_2, ARMAZEM_3
};
enum {
	SELECIONADO, NAO_SELECIONADO
};
enum {
	PEQUENA, GRANDE
};
enum {
	COMIDA_1, COMIDA_2, COMIDA_3, COMIDA_4, SEM_COMIDA
};
enum {
	LOCAL_1, LOCAL_2, LOCAL_3, LOCAL_4
};
enum {
	POSICAO_1, POSICAO_2, POSICAO_3, POSICAO_4
};
enum {
	SUBIR, DESCER, ESQUERDA, DIREITA, PEGAR, SAIR, NADA
};
enum {
	T10, T20, T30, T40, T50, T60, T70, T80, T90, T100, T110, T120, T130, T140, T150, TOTAL_FONTES
};
//

/*-------------------------------- STRUCTS ------------------------------------*/

typedef struct Controle {
	bool menu, jogar, pausa, sair, reiniciar, musica, sfx;
	double tempoTotal, tempoExecucao;
	int opcao;
	clock_t tempoInicial, tempoFinal;
	ALLEGRO_EVENT_QUEUE *fila_eventos;
	ALLEGRO_FONT *fonte[TOTAL_FONTES];
	ALLEGRO_DISPLAY *display;
	ALLEGRO_BITMAP *imgPlaca, *imgMenu, *imgBota, *imgTocha, *imgPa, *imgRelogio, *imgSelecao[2], *imgRecordes;
	ALLEGRO_SAMPLE *somselect, *somenter;
};

typedef struct Formiga {
	int comidaAtual, x, y, DIRECAO, tempoVelocidade, velocidade;
	bool vazio;
	ALLEGRO_BITMAP *imgFormiga[4];
};

typedef struct Mapa {
	int armazem[QUANTIDADE_ARMAZENS][QUANTIDADE_LOCAIS], **mapa, x, y;
	ALLEGRO_BITMAP *imgParede, *imgCaminho, *imgBackground, *imgComida[4], *imgSombra[2], *imgComidaAmazem[4], *imgArmazem;
};

typedef struct Item {
	int quantidadePa;
	bool tocha, bota;
	double tempoTocha, tempoFinalTocha, tempoInicialTocha, tempoBota, tempoFinalBota, tempoInicialBota;
	ALLEGRO_BITMAP *imgPa, *imgBota, *imgTocha, *imgKit;
};

/*-----------------------------------------------------------------------------*/

// Inicia o tempo dos itens (CARLOS E CAIO)
void tempoItem(bool inicio, Item &i) {
	if (inicio) {
		i.tempoInicialBota = clock();
		i.tempoInicialTocha = clock();
	}
	else {
		// Decorre o tempo para que os itens não se tornem eternos
		i.tempoFinalBota = clock();
		i.tempoFinalTocha = clock();
		i.tempoBota -= (i.tempoFinalBota - i.tempoInicialBota) / (double)CLOCKS_PER_SEC;
		i.tempoTocha -= (i.tempoFinalTocha - i.tempoInicialTocha) / (double)CLOCKS_PER_SEC;
	}
}

// Sorteia a posição dos itens no Mapa (CARLOS E CAIO)
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

// Faz o menu principal
void menuPrincipal(Controle &c, const char *st1, const char *st2, const char *st3, const char *st4, bool &recordes, bool &musica, bool principal) {
	const int JOGAR = 0, RECORDES = 1, SOM = 2, SAIR = 3;
	const int MUSICA = 0, SFX = 1, VOLTAR = 2;
	int x = 0, y = -10;
	if (!al_is_event_queue_empty(c.fila_eventos)) {
		ALLEGRO_EVENT evento;
		al_wait_for_event(c.fila_eventos, &evento);
		if (evento.type == ALLEGRO_EVENT_KEY_CHAR) {
			switch (evento.keyboard.keycode)
			{
				// Caso a tecla W ou Seta para cima forem pressionadas, seleciona a opção de cima
			case ALLEGRO_KEY_W:
			case ALLEGRO_KEY_UP:
				c.opcao--;
				if (c.sfx) {
					c.somselect = al_load_sample("res/select.wav");
					al_play_sample(c.somselect, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				}	
				if (c.opcao < JOGAR)
					c.opcao = JOGAR;
				break;
				// Caso a tecla S ou Seta para baixo forem pressionadas, seleciona a opção de baixo
			case ALLEGRO_KEY_S:
			case ALLEGRO_KEY_DOWN:
				c.opcao++;
				if (c.sfx) {
					c.somselect = al_load_sample("res/select.wav");
					al_play_sample(c.somselect, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
				else {

				}
				if (c.opcao > SAIR)
					c.opcao = SAIR;
				break;
			case ALLEGRO_KEY_ENTER:
			case ALLEGRO_KEY_SPACE:
				if (c.sfx) {
					c.somenter = al_load_sample("res/enter.wav");
					al_play_sample(c.somenter, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
				// Caso as teclas Enter ou Espaço forem pressionadas em Inciar, finaliza o Menu e inicia o jogo
				if (c.opcao == JOGAR) {
					c.menu = false;
					c.jogar = true;
				}
				// Caso as teclas Enter ou Espaço forem pressionadas em Recordes, entra no Menu de recordes
				else if (c.opcao == RECORDES) {
					recordes = true;
				}
				// Caso as teclas Enter ou Espaço forem pressionadas em Musica, entra no Menu de musicas
				else if (c.opcao == SOM) {
					musica = true;
					c.opcao = MUSICA;
				}
				// Caso as teclas Enter ou Espaço forem pressionadas em Sair, finaliza o jogo
				else if (c.opcao == SAIR) {
					if (principal)
						c.sair = true;
					else
						c.reiniciar = true;
				}
				break;
			}
		}
	}
	// Posiciona as opções do Menu
	if (c.opcao == JOGAR) {
		al_draw_bitmap(c.imgSelecao[SELECIONADO], x + 305, y + 350, NULL);
		al_draw_text(c.fonte[T50], al_map_rgb(0, 0, 0), x + 365, y + 360, NULL, st1);
	}
	else {
		al_draw_bitmap(c.imgSelecao[NAO_SELECIONADO], x + 305, y + 350, NULL);
		al_draw_text(c.fonte[T50], al_map_rgb(255, 255, 255), x + 365, y + 360, NULL, st1);
	}

	if (c.opcao == RECORDES) {
		al_draw_bitmap(c.imgSelecao[SELECIONADO], x + 305, y + 450, NULL);
		al_draw_text(c.fonte[T50], al_map_rgb(0, 0, 0), x + 350, y + 460, NULL, st2);
	}
	else {
		al_draw_bitmap(c.imgSelecao[NAO_SELECIONADO], x + 305, y + 450, NULL);
		al_draw_text(c.fonte[T50], al_map_rgb(255, 255, 255), x + 350, y + 460, NULL, st2);
	}

	if (c.opcao == SOM) {
		al_draw_bitmap(c.imgSelecao[SELECIONADO], x + 305, y + 550, NULL);
		al_draw_text(c.fonte[T50], al_map_rgb(0, 0, 0), x + 400, y + 560, NULL, st3);
	}
	else {
		al_draw_bitmap(c.imgSelecao[NAO_SELECIONADO], x + 305, y + 550, NULL);
		al_draw_text(c.fonte[T50], al_map_rgb(255, 255, 255), x + 400, y + 560, NULL, st3);
	}

	if (c.opcao == SAIR) {
		al_draw_bitmap(c.imgSelecao[SELECIONADO], x + 305, y + 650, NULL);
		al_draw_text(c.fonte[T50], al_map_rgb(0, 0, 0), x + 410, y + 660, NULL, st4);
	}
	else {
		al_draw_bitmap(c.imgSelecao[NAO_SELECIONADO], x + 305, y + 650, NULL);
		al_draw_text(c.fonte[T50], al_map_rgb(255, 255, 255), x + 410, y + 660, NULL, st4);
	}
}

// Faz o menu recordes
void menuRecordes(Controle c, bool &recordes) {
	const int JOGAR = 0, RECORDES = 1, SOM = 2, SAIR = 3;
	const int MUSICA = 0, SFX = 1, VOLTAR = 2;
	int x = 0, y = 0;
	if (!al_is_event_queue_empty(c.fila_eventos)) {
		ALLEGRO_EVENT evento;
		al_wait_for_event(c.fila_eventos, &evento);
		if (evento.type == ALLEGRO_EVENT_KEY_CHAR) {
			switch (evento.keyboard.keycode)
			{
			case ALLEGRO_KEY_ENTER:
			case ALLEGRO_KEY_SPACE:
				recordes = false;
				c.opcao = JOGAR;
				break;
			}
		}
	}
	// Posiciona as opções do Menu
	al_draw_bitmap(c.imgRecordes, x + LARGURA / 2 - 145, y + 330, NULL);
	al_draw_bitmap(c.imgSelecao[SELECIONADO], x + 305, y + 650, NULL);
	al_draw_text(c.fonte[T50], al_map_rgb(0, 0, 0), x + 400, y + 660, NULL, "back");
}

// Faz o menu musica
void menuMusica(Controle &c, bool &musica) {
	const int JOGAR = 0, RECORDES = 1, SOM = 2, SAIR = 3;
	const int MUSICA = 0, SFX = 1, VOLTAR = 2;
	int x = 0, y = 40;
	if (!al_is_event_queue_empty(c.fila_eventos)) {
		ALLEGRO_EVENT evento;
		al_wait_for_event(c.fila_eventos, &evento);
		if (evento.type == ALLEGRO_EVENT_KEY_CHAR) {
			switch (evento.keyboard.keycode)
			{
			case ALLEGRO_KEY_W:
			case ALLEGRO_KEY_UP:
				c.opcao--;
				if (c.opcao < MUSICA)
					c.opcao = MUSICA;
				break;
			case ALLEGRO_KEY_S:
			case ALLEGRO_KEY_DOWN:
				c.opcao++;
				if (c.opcao > VOLTAR)
					c.opcao = VOLTAR;
				break;
			case ALLEGRO_KEY_ENTER:
			case ALLEGRO_KEY_SPACE:
				if (c.opcao == MUSICA) {
					if (c.musica)
						c.musica = false;
					else
						c.musica = true;
				}
				else if (c.opcao == SFX) {
					if (c.sfx)
						c.sfx = false;
					else
						c.sfx = true;
				}
				else if (c.opcao == VOLTAR) {
					musica = false;
					c.opcao = JOGAR;
				}
				break;
			}
		}
	}
	// Posiciona as opções do Menu
	if (c.opcao == MUSICA) {
		al_draw_bitmap(c.imgSelecao[SELECIONADO], x + 305, y + 350, NULL);
		if (c.musica)
			al_draw_text(c.fonte[T50], al_map_rgb(0, 0, 0), x + 350, y + 360, NULL, "music: on");
		else
			al_draw_text(c.fonte[T50], al_map_rgb(0, 0, 0), x + 350, y + 360, NULL, "music: off");
	}
	else {
		al_draw_bitmap(c.imgSelecao[NAO_SELECIONADO], x + 305, y + 350, NULL);
		if (c.musica)
			al_draw_text(c.fonte[T50], al_map_rgb(255, 255, 255), x + 350, y + 360, NULL, "music: on");
		else
			al_draw_text(c.fonte[T50], al_map_rgb(255, 255, 255), x + 350, y + 360, NULL, "music: off");
	}

	if (c.opcao == SFX) {
		al_draw_bitmap(c.imgSelecao[SELECIONADO], x + 305, y + 450, NULL);
		if (c.sfx)
			al_draw_text(c.fonte[T50], al_map_rgb(0, 0, 0), x + 390, y + 460, NULL, "sfx: on");
		else
			al_draw_text(c.fonte[T50], al_map_rgb(0, 0, 0), x + 390, y + 460, NULL, "sfx: off");
	}
	else {
		al_draw_bitmap(c.imgSelecao[NAO_SELECIONADO], x + 305, y + 450, NULL);
		if (c.sfx)
			al_draw_text(c.fonte[T50], al_map_rgb(255, 255, 255), x + 390, y + 460, NULL, "sfx: on");
		else
			al_draw_text(c.fonte[T50], al_map_rgb(255, 255, 255), x + 390, y + 460, NULL, "sfx: off");
	}

	if (c.opcao == VOLTAR) {
		al_draw_bitmap(c.imgSelecao[SELECIONADO], x + 305, y + 550, NULL);
		al_draw_text(c.fonte[T50], al_map_rgb(0, 0, 0), x + 400, y + 560, NULL, "back");
	}
	else {
		al_draw_bitmap(c.imgSelecao[NAO_SELECIONADO], x + 305, y + 560, NULL);
		al_draw_text(c.fonte[T50], al_map_rgb(255, 255, 255), x + 400, y + 560, NULL, "back");
	}
}

// Apresenta a tela de menu ao jogador (CARLOS E CAIO)
void menu(Controle &c) {
	static bool recordes = false, musica = false;
	al_draw_bitmap(c.imgMenu, 0, 0, NULL);
	// Entra no Menu de Recordes
	if (recordes) {
		menuRecordes(c, recordes);
	}
	// Entra  no menu música
	else if (musica) {
		menuMusica(c, musica);
	}
	else {
		menuPrincipal(c, "   play", "hi-scores", "sound", "exit", recordes, musica, true);
	}
}

// Menus de pausa (CARLOS E MATEUS)
void pausa(Mapa &m, Controle &c, Formiga &f, Item &i) {
	static bool recordes = false, musica = false;
	al_draw_bitmap(c.imgMenu, 0, 0, NULL);
	// Entra no Menu de Recordes
	if (recordes) {
		menuRecordes(c, recordes);
	}
	// Entra  no menu música
	else if (musica) {
		menuMusica(c, musica);
	}
	else {
		menuPrincipal(c, "continue", "hi-scores", "music", "back", recordes, musica, false);
	}
}

// Verifica se a formiga está ou não com comida (CARLOS)
void formigaAtual(Mapa &m, Formiga f) {
	if (f.vazio)
		m.mapa[f.y][f.x] = FORMIGA_VAZIO;
	else
		m.mapa[f.y][f.x] = FORMIGA_CHEIO;
}

// Inicializa os armazens (CARLOS)
void iniciarArmazem(int armazem[QUANTIDADE_ARMAZENS][QUANTIDADE_LOCAIS]) {
	int COMIDA = COMIDA_4;
	for (size_t ARMAZEM = ARMAZEM_1; ARMAZEM < QUANTIDADE_ARMAZENS; ARMAZEM++) {
		for (size_t LOCAL = LOCAL_1; LOCAL < QUANTIDADE_LOCAIS; LOCAL++) {
			if (ARMAZEM == ARMAZEM_1) {
				armazem[ARMAZEM][LOCAL] = COMIDA;
				COMIDA--;
			}
			else {
				armazem[ARMAZEM][LOCAL] = SEM_COMIDA;
			}
		}
	}
}

// Gera um novo mapa (CARLOS)
void novoMapa(Mapa &m) {
	list <pair <int, int> > drillers;
	int **map = NULL;
	int qtdCaminhos;
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
	do {
		qtdCaminhos = 0;
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

		// faz a contagem da quantidade de CAMINHOS para que não gere um mapa muito pequeno
		for (size_t x = 0; x < m.x; x++)
			for (size_t y = 0; y < m.y; y++)
				if (map[y][x] == CAMINHO)
					qtdCaminhos++;
		if (qtdCaminhos > 75)
			novoMapa = false;
		else
			delete map;

	} while (novoMapa);

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

// Colcoa uma nova formiga no mapa (CARLOS)
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

// Função que verifica se vai retirar ou colocar a comida no armazém (CARLOS)
void verificarArmazem(Formiga &f, Mapa &m, int ARMAZEM) {
	int LOCAL;
	bool retirar = false;

	if (f.comidaAtual == SEM_COMIDA) { // Se a formiga não tiver segurando comida, significa que ela vai pegar do armazém
		for (LOCAL = LOCAL_4; LOCAL >= LOCAL_1; LOCAL--) {
			if (m.armazem[ARMAZEM][LOCAL] != SEM_COMIDA) {
				f.comidaAtual = m.armazem[ARMAZEM][LOCAL];
				m.armazem[ARMAZEM][LOCAL] = SEM_COMIDA;
				m.mapa[f.y][f.x] = FORMIGA_CHEIO;
				f.vazio = false;
				LOCAL = -1;
			}
		}
	}
	else { // Se já tiver, ela vai colocar no armazém
		for (LOCAL = LOCAL_1; LOCAL <= LOCAL_4; LOCAL++) {
			if (m.armazem[ARMAZEM][LOCAL] == SEM_COMIDA) {
				if (LOCAL == LOCAL_1)
					retirar = true;
				else if (f.comidaAtual < m.armazem[ARMAZEM][LOCAL - 1])
					retirar = true;

				if (retirar) {
					m.armazem[ARMAZEM][LOCAL] = f.comidaAtual;
					f.comidaAtual = SEM_COMIDA;
					m.mapa[f.y][f.x] = FORMIGA_VAZIO;
					f.vazio = true;
				}
				LOCAL = 4;
			}
		}
	}
}

// Nessa função são executados os comandos do jogo (CARLOS, CAIO E MATEUS)
// Obs.: Todos os comentários dessa função servem para todas as posições pressionadas, apenas para redução de linhas.
void lerComandos(Mapa &m, Controle &c, Formiga &f, Item &i) {
	if (!al_is_event_queue_empty(c.fila_eventos)) {
		ALLEGRO_EVENT evento;
		al_wait_for_event(c.fila_eventos, &evento);
		if (evento.type == ALLEGRO_EVENT_KEY_CHAR) {
			switch (evento.keyboard.keycode)
			{
			case ALLEGRO_KEY_W:
			case ALLEGRO_KEY_UP:
				if (clock() - f.tempoVelocidade > f.velocidade) {
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
					f.tempoVelocidade = clock();
				}
				f.DIRECAO = SUBIR;
				break;
			case ALLEGRO_KEY_S:
			case ALLEGRO_KEY_DOWN:
				if (clock() - f.tempoVelocidade > f.velocidade) {
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
					f.tempoVelocidade = clock();
				}
				f.DIRECAO = DESCER;
				break;
			case ALLEGRO_KEY_A:
			case ALLEGRO_KEY_LEFT:
				if (clock() - f.tempoVelocidade > f.velocidade) {
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
					f.tempoVelocidade = clock();
				}
				f.DIRECAO = ESQUERDA;
				break;
			case ALLEGRO_KEY_D:
			case ALLEGRO_KEY_RIGHT:
				if (clock() - f.tempoVelocidade > f.velocidade) {
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
					f.tempoVelocidade = clock();
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

// Nessa função a matriz é percorrida e os números são substituidos (CARLOS)
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
			// Renderiza a imagem do Caminho
			else if (m.mapa[i][j] == CAMINHO) {
				al_draw_bitmap(m.imgCaminho, x, y, NULL);
				al_draw_bitmap(m.imgCaminho, x, y, NULL);
			}
			// Renderiza a imagem do Item Pá
			else if (m.mapa[i][j] == ITEM_PA) {
				al_draw_bitmap(m.imgCaminho, x, y, NULL);
				al_draw_bitmap(it.imgPa, x, y, NULL);
			}
			// Renderiza a imagem do Item Bota
			else if (m.mapa[i][j] == ITEM_BOTA) {
				al_draw_bitmap(m.imgCaminho, x, y, NULL);
				al_draw_bitmap(it.imgBota, x, y, NULL);
			}
			// Renderiza a imagem do Item Tocha
			else if (m.mapa[i][j] == ITEM_TOCHA) {
				al_draw_bitmap(m.imgCaminho, x, y, NULL);
				al_draw_bitmap(it.imgTocha, x, y, NULL);
			}
			// Renderiza a imagem do Item Kit de Reparos
			else if (m.mapa[i][j] == ITEM_KIT) {
				al_draw_bitmap(m.imgCaminho, x, y, NULL);
				al_draw_bitmap(it.imgKit, x, y, NULL);
			}
			// Renderiza a imagem dos Armazens
			else if (m.mapa[i][j] == ARMAZEM_MAPA_1) {
				al_draw_bitmap(m.imgCaminho, x, y, NULL);
				al_draw_bitmap(m.imgArmazem, x, y, NULL);
				// Renderiza a pilha de comidas dentro do armazém
				if (m.armazem[ARMAZEM_1][LOCAL_1] == COMIDA_1)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_1], x, y, NULL);
				else if (m.armazem[ARMAZEM_1][LOCAL_1] == COMIDA_2)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_2], x, y, NULL);
				else if (m.armazem[ARMAZEM_1][LOCAL_1] == COMIDA_3)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_3], x, y, NULL);
				else if (m.armazem[ARMAZEM_1][LOCAL_1] == COMIDA_4)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_4], x, y, NULL);

				if (m.armazem[ARMAZEM_1][LOCAL_2] == COMIDA_1)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_1], x, y, NULL);
				else if (m.armazem[ARMAZEM_1][LOCAL_2] == COMIDA_2)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_2], x, y, NULL);
				else if (m.armazem[ARMAZEM_1][LOCAL_2] == COMIDA_3)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_3], x, y, NULL);
				else if (m.armazem[ARMAZEM_1][LOCAL_2] == COMIDA_4)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_4], x, y, NULL);

				if (m.armazem[ARMAZEM_1][LOCAL_3] == COMIDA_1)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_1], x, y, NULL);
				else if (m.armazem[ARMAZEM_1][LOCAL_3] == COMIDA_2)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_2], x, y, NULL);
				else if (m.armazem[ARMAZEM_1][LOCAL_3] == COMIDA_3)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_3], x, y, NULL);
				else if (m.armazem[ARMAZEM_1][LOCAL_3] == COMIDA_4)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_4], x, y, NULL);

				if (m.armazem[ARMAZEM_1][LOCAL_4] == COMIDA_1)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_1], x, y, NULL);
				else if (m.armazem[ARMAZEM_1][LOCAL_4] == COMIDA_2)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_2], x, y, NULL);
				else if (m.armazem[ARMAZEM_1][LOCAL_4] == COMIDA_3)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_3], x, y, NULL);
				else if (m.armazem[ARMAZEM_1][LOCAL_4] == COMIDA_4)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_4], x, y, NULL);
			}
			else if (m.mapa[i][j] == ARMAZEM_MAPA_2) {
				al_draw_bitmap(m.imgCaminho, x, y, NULL);
				al_draw_bitmap(m.imgArmazem, x, y, NULL);
				if (m.armazem[ARMAZEM_2][LOCAL_1] == COMIDA_1)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_1], x, y, NULL);
				else if (m.armazem[ARMAZEM_2][LOCAL_1] == COMIDA_2)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_2], x, y, NULL);
				else if (m.armazem[ARMAZEM_2][LOCAL_1] == COMIDA_3)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_3], x, y, NULL);
				else if (m.armazem[ARMAZEM_2][LOCAL_1] == COMIDA_4)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_4], x, y, NULL);

				if (m.armazem[ARMAZEM_2][LOCAL_2] == COMIDA_1)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_1], x, y, NULL);
				else if (m.armazem[ARMAZEM_2][LOCAL_2] == COMIDA_2)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_2], x, y, NULL);
				else if (m.armazem[ARMAZEM_2][LOCAL_2] == COMIDA_3)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_3], x, y, NULL);
				else if (m.armazem[ARMAZEM_2][LOCAL_2] == COMIDA_4)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_4], x, y, NULL);

				if (m.armazem[ARMAZEM_2][LOCAL_3] == COMIDA_1)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_1], x, y, NULL);
				else if (m.armazem[ARMAZEM_2][LOCAL_3] == COMIDA_2)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_2], x, y, NULL);
				else if (m.armazem[ARMAZEM_2][LOCAL_3] == COMIDA_3)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_3], x, y, NULL);
				else if (m.armazem[ARMAZEM_2][LOCAL_3] == COMIDA_4)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_4], x, y, NULL);

				if (m.armazem[ARMAZEM_2][LOCAL_4] == COMIDA_1)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_1], x, y, NULL);
				else if (m.armazem[ARMAZEM_2][LOCAL_4] == COMIDA_2)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_2], x, y, NULL);
				else if (m.armazem[ARMAZEM_2][LOCAL_4] == COMIDA_3)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_3], x, y, NULL);
				else if (m.armazem[ARMAZEM_2][LOCAL_4] == COMIDA_4)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_4], x, y, NULL);
			}
			else if (m.mapa[i][j] == ARMAZEM_MAPA_3) {
				al_draw_bitmap(m.imgCaminho, x, y, NULL);
				al_draw_bitmap(m.imgArmazem, x, y, NULL);
				if (m.armazem[ARMAZEM_3][LOCAL_1] == COMIDA_1)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_1], x, y, NULL);
				else if (m.armazem[ARMAZEM_3][LOCAL_1] == COMIDA_2)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_2], x, y, NULL);
				else if (m.armazem[ARMAZEM_3][LOCAL_1] == COMIDA_3)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_3], x, y, NULL);
				else if (m.armazem[ARMAZEM_3][LOCAL_1] == COMIDA_4)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_4], x, y, NULL);

				if (m.armazem[ARMAZEM_3][LOCAL_2] == COMIDA_1)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_1], x, y, NULL);
				else if (m.armazem[ARMAZEM_3][LOCAL_2] == COMIDA_2)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_2], x, y, NULL);
				else if (m.armazem[ARMAZEM_3][LOCAL_2] == COMIDA_3)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_3], x, y, NULL);
				else if (m.armazem[ARMAZEM_3][LOCAL_2] == COMIDA_4)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_4], x, y, NULL);

				if (m.armazem[ARMAZEM_3][LOCAL_3] == COMIDA_1)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_1], x, y, NULL);
				else if (m.armazem[ARMAZEM_3][LOCAL_3] == COMIDA_2)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_2], x, y, NULL);
				else if (m.armazem[ARMAZEM_3][LOCAL_3] == COMIDA_3)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_3], x, y, NULL);
				else if (m.armazem[ARMAZEM_3][LOCAL_3] == COMIDA_4)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_4], x, y, NULL);

				if (m.armazem[ARMAZEM_3][LOCAL_4] == COMIDA_1)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_1], x, y, NULL);
				else if (m.armazem[ARMAZEM_3][LOCAL_4] == COMIDA_2)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_2], x, y, NULL);
				else if (m.armazem[ARMAZEM_3][LOCAL_4] == COMIDA_3)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_3], x, y, NULL);
				else if (m.armazem[ARMAZEM_3][LOCAL_4] == COMIDA_4)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_4], x, y, NULL);
			}
			// Renderiza a formiga sem comida
			else if (m.mapa[i][j] == FORMIGA_VAZIO) {
				sombraX = x + TILE / 2 - LARGURA;
				sombraY = y + TILE / 2 + PLACAR - ALTURA;
				al_draw_bitmap(m.imgCaminho, x, y, NULL);
				al_draw_bitmap(f.imgFormiga[f.DIRECAO], x, y, NULL);
			}
			// Renderiza a formiga com comida
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
	// Aumenta o tamanho da visão da formiga, efeito do item TOCHA
	//if (it.tocha)
	//	al_draw_bitmap(m.imgSombra[GRANDE], sombraX, sombraY, NULL);
	// Renderiza o efeito que impede a visão da formiga
	//else
	//	al_draw_bitmap(m.imgSombra[PEQUENA], sombraX, sombraY, NULL);
	/*---------------------------- HUD -------------------------------*/
	al_draw_bitmap(c.imgPlaca, 0, 0, NULL);

	al_draw_bitmap(c.imgRelogio, 490, 20, NULL);
	al_draw_text(c.fonte[T10], al_map_rgb(0, 0, 0), 510, 20, NULL, "time until earthquake: ");
	al_draw_text(c.fonte[T10], al_map_rgb(0, 0, 0), 680, 20, NULL, tempoChar);

	al_draw_bitmap(c.imgBota, 490, 35, NULL);
	al_draw_text(c.fonte[T10], al_map_rgb(0, 0, 0), 510, 35, NULL, "boot: ");
	if (it.bota)
		al_draw_text(c.fonte[T10], al_map_rgb(0, 0, 0), 550, 35, NULL, tempoBota);
	else
		al_draw_text(c.fonte[T10], al_map_rgb(0, 0, 0), 550, 35, NULL, "no item");

	al_draw_bitmap(c.imgTocha, 490, 50, NULL);
	al_draw_text(c.fonte[T10], al_map_rgb(0, 0, 0), 510, 50, NULL, "torch: ");
	if (it.tocha)
		al_draw_text(c.fonte[T10], al_map_rgb(0, 0, 0), 558, 50, NULL, tempoTocha);
	else
		al_draw_text(c.fonte[T10], al_map_rgb(0, 0, 0), 558, 50, NULL, "no item");

	al_draw_bitmap(c.imgPa, 490, 65, NULL);
	al_draw_text(c.fonte[T10], al_map_rgb(0, 0, 0), 510, 65, NULL, "spade: ");
	al_draw_text(c.fonte[T10], al_map_rgb(0, 0, 0), 558, 65, NULL, qtdPa);
	/*----------------------------------------------------------------*/
}

// Carrega o modo gráfico (CARLOS)
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

//Carrega os recursos do jogo (CARLOS)
void carregarRecursos(Mapa &m, Controle &c, Formiga &f, Item &i) {
	int tamCont = 10;
	c.imgPlaca = al_load_bitmap("res/PLACA.png");
	c.imgMenu = al_load_bitmap("res/MENU.png");
	c.imgBota = al_load_bitmap("res/ITEM_BOTA_HUD.png");
	c.imgPa = al_load_bitmap("res/ITEM_PA_HUD.png");
	c.imgTocha = al_load_bitmap("res/ITEM_TOCHA_HUD.png");
	c.imgRelogio = al_load_bitmap("res/RELOGIO_HUD.png");
	c.imgSelecao[SELECIONADO] = al_load_bitmap("res/OPCAO_CLARO.png");
	c.imgSelecao[NAO_SELECIONADO] = al_load_bitmap("res/OPCAO_ESCURO.png");
	c.imgRecordes = al_load_bitmap("res/RECORDES.png");
	c.somselect = al_load_sample("res/select.wav");
	c.somenter = al_load_sample("res/enter.wav");
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
	m.imgArmazem = al_load_bitmap("res/ARMAZEM.png");
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

// Verifica se o jogador venceu (CARLOS)
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

// Tela de fim de jogo (CARLOS)
void fimJogo(Controle &c) {
	int x = 180, y = 260;
	string tempoString = to_string(c.tempoExecucao);
	char const* tempoChar = tempoString.c_str();
	al_draw_bitmap(c.imgMenu, 0, 0, NULL);
	al_draw_filled_rounded_rectangle(60, 350, 900, 550, 25, 25, al_map_rgba(200, 200, 200, 100));
	al_draw_text(c.fonte[T50], al_map_rgb(0, 0, 0), x + 60, y + 125, NULL, "congratulations!");
	al_draw_text(c.fonte[T50], al_map_rgb(0, 0, 0), 80, y + 200, NULL, "Your time has been: ");
	al_draw_text(c.fonte[T50], al_map_rgb(0, 0, 0), x + 470, y + 200, NULL, tempoChar);
	al_draw_bitmap(c.imgSelecao[SELECIONADO], LARGURA / 2 - 180, y + 380, NULL);
	al_draw_text(c.fonte[T20], al_map_rgb(0, 0, 0), x + 145, y + 400, NULL, "press enter to continue");
	c.jogar = false;
	if (!al_is_event_queue_empty(c.fila_eventos)) {
		ALLEGRO_EVENT evento;
		al_wait_for_event(c.fila_eventos, &evento);
		if (evento.type == ALLEGRO_EVENT_KEY_CHAR) {
			switch (evento.keyboard.keycode)
			{
			case ALLEGRO_KEY_ENTER:
				c.reiniciar = true;
			}
		}
	}
}
// Inicializa todas as variaveis do programa e atribui seus valores (CARLOS)
void inicializarVariaveis(Mapa &m, Controle &c, Formiga &f, Item &i) {
	c.opcao = NULL;
	c.menu = true;
	c.jogar = false;
	c.pausa = false;
	c.sair = false;
	c.reiniciar = false;
	c.musica = true;
	c.sfx = true;
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

// Finaliza o programa, destruindo todos os objetos criados (CARLOS)
void finalizar(Mapa &m, Controle &c, Formiga &f, Item &i) {
	al_destroy_display(c.display);
	al_destroy_event_queue(c.fila_eventos);
	al_destroy_bitmap(c.imgBota);
	al_destroy_bitmap(c.imgPa);
	al_destroy_bitmap(c.imgTocha);
	al_destroy_bitmap(c.imgRelogio);
	al_destroy_bitmap(c.imgSelecao[SELECIONADO]);
	al_destroy_bitmap(c.imgSelecao[NAO_SELECIONADO]);
	al_destroy_bitmap(c.imgRecordes);
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
	al_destroy_bitmap(m.imgArmazem);
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

// Sorteia os armazém pelo jogo (CARLOS E MATEUS)
void posicionarArmazens(Mapa &m) {
	bool sorteia = true;
	int x, y;
	for (size_t ARMAZEM = ARMAZEM_MAPA_1; ARMAZEM <= ARMAZEM_MAPA_3; ARMAZEM++) {
		while (sorteia) {
			x = rand() % (m.x);
			y = rand() % (m.y);
			if (m.mapa[y][x] == PAREDE) {
				if (m.mapa[y - 1][x] == CAMINHO || m.mapa[y][x - 1] == CAMINHO || m.mapa[y + 1][x] == CAMINHO || m.mapa[y][x + 1] == CAMINHO) {
					sorteia = false;
					m.mapa[y][x] = ARMAZEM;
					x = 0;
					y = 0;
				}
			}
		}
		sorteia = true;
	}
}
void toca(Controle &c) {

	ALLEGRO_SAMPLE *sample = NULL;

	if (!al_install_audio()) {
		fprintf(stderr, "failed to initialize audio!\n");
	}

	if (!al_init_acodec_addon()) {
		fprintf(stderr, "failed to initialize audio codecs!\n");
	}

	if (!al_reserve_samples(1)) {
		fprintf(stderr, "failed to reserve samples!\n");
	}

	sample = al_load_sample("res/main_theme.wav");


	/* Loop the sample until the display closes. */
	//al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);

}

// Reinicia as variáveis (CARLOS)
void inicializarJogo(Mapa &m, Controle &c, Formiga &f, Item &i) {
	inicializarVariaveis(m, c, f, i);
	novoMapa(m);
	posicionarFormiga(m, f);
	iniciarArmazem(m.armazem);
	posicionarItens(m, i);
	posicionarArmazens(m);
	toca(c);
}

// função para medir tempo e sortear novo mapa (CARLOS E MATEUS)
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
			posicionarItens(m, i);
			posicionarArmazens(m);
			toca(c);
			c.tempoTotal = TEMPO_MAXIMO;
		}
	}
}

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

		//Inicia o menu do jogo
		if (c.menu) {
			menu(c);
		}
		// Inicia o jogo chamando todas as funções
		else if (c.jogar) {
			tempoItem(true, i);
			medirTempo(true, m, c, f, i);
			lerComandos(m, c, f, i);
			desenharFrame(m, c, f, i);
			medirTempo(false, m, c, f, i);
			tempoItem(false, i);
		}
		// Ativa o menu de pausa
		else if (c.pausa) {
			pausa(m, c, f, i);
		}
		// Finaliza o jogo após vencer
		if (venceu(m.armazem) && f.vazio)
			fimJogo(c);
		if (c.reiniciar)
			inicializarJogo(m, c, f, i);

		al_flip_display();
	}

	/*-------------------------------------------------------------------------*/

	finalizar(m, c, f, i);

	return 0;
}
//
