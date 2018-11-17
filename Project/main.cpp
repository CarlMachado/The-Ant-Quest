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
//
using namespace std;

// CONSTANTES
//
#define LARGURA 960
#define ALTURA 740

#define TEMPO_MAXIMO 2.0

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
	FORMIGA_CHEIO 
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
//

/*-------------------------------- STRUCTS ------------------------------------*/

typedef struct Controle {
	bool menu = false, jogar = true, pausa = false, sair = false, reiniciar = false;
	clock_t tempoInicial = 0, tempoFinal = 0;
	double tempoTotal = TEMPO_MAXIMO, tempoExecucao = 0;
	ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_BITMAP *imgPlaca;
	ALLEGRO_BITMAP *imgMenu;
	ALLEGRO_FONT *fonte10;
};

typedef struct Formiga {
	int comidaAtual = SEM_COMIDA;
	int x, y, DIRECAO = SUBIR;
	float velocidade = 5.0;
	bool vazio = true;
	ALLEGRO_BITMAP *imgFormiga[4];
};

typedef struct Mapa {
	int armazem[QUANTIDADE_ARMAZENS][QUANTIDADE_LOCAIS];
	int **mapa;
	int x, y;
	ALLEGRO_BITMAP *imgParede;
	ALLEGRO_BITMAP *imgCaminho;
	ALLEGRO_BITMAP *imgBackground;
	ALLEGRO_BITMAP *imgComida;
	ALLEGRO_BITMAP *imgSombra[2];
	ALLEGRO_BITMAP *imgComidaAmazem[4];
	ALLEGRO_BITMAP *imgArmazem[2];
};

typedef struct Item {
	int quantidadePa = 0;
	bool tocha = false;
	bool kitReparos = false;
	bool bota = false;

	ALLEGRO_BITMAP *imgPa;
	ALLEGRO_BITMAP *imgBota;
	ALLEGRO_BITMAP *imgTocha;
	ALLEGRO_BITMAP *imgKit;
};

/*-----------------------------------------------------------------------------*/




// PARTE DO CÓDIGO ESCRITA POR CAIO
//
// Apresenta a tela de menu ao jogador
void menu(Controle &c) {
	int tecla = NADA;

	while(!al_is_event_queue_empty(c.fila_eventos)) {
		ALLEGRO_EVENT evento;
		al_wait_for_event(c.fila_eventos, &evento);
		if(evento.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch(evento.keyboard.keycode)
			{
			case ALLEGRO_KEY_UP:
				tecla = SUBIR;
				break;
			case ALLEGRO_KEY_DOWN:
				tecla = DESCER;
				break;
			case ALLEGRO_KEY_SPACE:
				tecla = PEGAR;
			}
		} else if(evento.type == ALLEGRO_EVENT_KEY_UP) {
			tecla = NADA;
		} else if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			c.sair = true;
		}
	}

	switch (tecla)
	{
	case SUBIR:

		break;
	case DESCER:

		break;
	case PEGAR:

		break;
	}

	//LÓGICA MENU

}
//
// FIM DA PARTE DE CÓDIGO ESCRITA POR CAIO



/*---------------------------------------------------------------------------------------------------*/



// PARTE DO CÓDIGO ESCRITA POR CARLOS
//
// Verifica se a formiga está ou não com comida
void formigaAtual(Mapa &m, Formiga f) {
	if(f.vazio)
		m.mapa[f.y][f.x] = FORMIGA_VAZIO;
	else
		m.mapa[f.y][f.x] = FORMIGA_CHEIO;
}

// Inicializa os armazens
void iniciarArmazem(int armazem[QUANTIDADE_ARMAZENS][QUANTIDADE_LOCAIS]) {
	int COMIDA = 4;
	for (size_t ARMAZEM = 0; ARMAZEM < QUANTIDADE_ARMAZENS; ARMAZEM++) {
		for (size_t LOCAL = 0; LOCAL < QUANTIDADE_LOCAIS; LOCAL++) {
			if (ARMAZEM == ARMAZEM_1) {
				armazem[ARMAZEM_1][LOCAL] = COMIDA;
				COMIDA--;
			} else {
				armazem[ARMAZEM][LOCAL] = SEM_COMIDA;
			}
		}
	}
}

// Cria um novo mapa
void novoMapa(Mapa &m) {
	list <pair <int, int> > drillers;
	int **map = NULL;
	int cont = 0;
	bool novoMapa = true;

	srand(time(NULL));

	m.x = 15 + (rand() % 8);
	if(m.x % 2 != 0)
		m.x++;
	m.y = (m.x / 3) * 2;
	if(m.y % 2 != 0)
		m.y++;

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
		for(size_t x = 0; x < m.x; x++)
			for(size_t y = 0; y < m.y; y++)
				if(map[y][x] == CAMINHO)
					cont++;
		if(cont > 79)
			novoMapa = false;

	}

	m.x += 2;
	m.y += 2;

	m.mapa = new int *[m.y];

	for(size_t y = 0; y < m.y; y++)
		m.mapa[y] = new int[m.x];

	for(size_t x = 0; x < m.x; x++)
		for(size_t y = 0; y < m.y; y++)
			m.mapa[y][x] = PAREDE;

	for(size_t x = 1; x < m.x - 1; x++)
		for(size_t y = 1; y < m.y - 1; y++)
			m.mapa[y][x] = map[y - 1][x - 1];

	delete map;
}

// Colcoa uma nova formiga no mapa
void posicionarFormiga(Mapa &m, Formiga &f) {
	bool sortear = true;
	srand(time(NULL));
	while(sortear) {
		f.x = rand() % m.x;
		f.y = rand() % m.y;
		if(m.mapa[f.y][f.x] == CAMINHO) {
			if (f.vazio)
				m.mapa[f.y][f.x] = FORMIGA_VAZIO;
			else
				m.mapa[f.y][f.x] = FORMIGA_CHEIO;
			sortear = false;
		}
	}
}
// Nessa função verifica-se se vai retirar ou colocar a comida no armazém
void verificarArmazem(Formiga &f, Mapa &m, int ARMAZEM) {
	int LOCAL;
	bool retirar = false;

	if(f.comidaAtual == 0) { // Se a formiga não tiver segurando comida, significa que ela vai pegar do armazém
		for (LOCAL = 3; LOCAL >= 0; LOCAL--) {
			if (m.armazem[ARMAZEM][LOCAL] != SEM_COMIDA) {
				f.comidaAtual = m.armazem[ARMAZEM][LOCAL];
				m.armazem[ARMAZEM][LOCAL] = SEM_COMIDA;
				m.mapa[f.x][f.y] = FORMIGA_CHEIO;
				f.vazio = false;
				LOCAL = -1;
			}
		}
	} else { // Se já tiver, ela vai colocar no armazém
		for(LOCAL = 0; LOCAL <= 3; LOCAL++) {
			if(m.armazem[ARMAZEM][LOCAL] == SEM_COMIDA) {
				if(LOCAL == 0)
					retirar = true;
				else if(f.comidaAtual < m.armazem[ARMAZEM][LOCAL - 1])
					retirar = true;

				if(retirar) {
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

// Nessa função são executados os comandos do jogo
void lerComandos(Mapa &m, Controle &c, Formiga &f) {
	int tecla = NADA;
	if(!al_is_event_queue_empty(c.fila_eventos)) {
		ALLEGRO_EVENT evento;
		al_wait_for_event(c.fila_eventos, &evento);
		if(evento.type == ALLEGRO_EVENT_KEY_CHAR) {
			switch (evento.keyboard.keycode)
			{
			case ALLEGRO_KEY_UP:
				tecla = SUBIR;
				f.DIRECAO = SUBIR;
				break;
			case ALLEGRO_KEY_DOWN:
				tecla = DESCER;
				f.DIRECAO = DESCER;
				break;
			case ALLEGRO_KEY_LEFT:
				tecla = ESQUERDA;
				f.DIRECAO = ESQUERDA;
				break;
			case ALLEGRO_KEY_RIGHT:
				tecla = DIREITA;
				f.DIRECAO = DIREITA;
				break;
			case ALLEGRO_KEY_ESCAPE:
				tecla = SAIR;
				break;
			case ALLEGRO_KEY_SPACE:
				tecla = PEGAR;
			}
		} else if (evento.type == ALLEGRO_EVENT_KEY_UP) {
			tecla = NADA;
		} else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			c.sair = true;
		}
	}
	
	switch(tecla)
	{
	case SUBIR:
		if(m.mapa[f.y - 1][f.x] == CAMINHO) {
			m.mapa[f.y][f.x] = CAMINHO;
			f.y--;
			formigaAtual(m, f);
		}
		break;
	case DESCER:
		if(m.mapa[f.y + 1][f.x] == CAMINHO) {
			m.mapa[f.y][f.x] = CAMINHO;
			f.y++;
			formigaAtual(m, f);
		}
		break;
	case ESQUERDA:
		if(m.mapa[f.y][f.x - 1] == CAMINHO) {
			m.mapa[f.y][f.x] = CAMINHO;
			f.x--;
			formigaAtual(m, f);
		}
		break;
	case DIREITA:
		if(m.mapa[f.y][f.x + 1] == CAMINHO) {
			m.mapa[f.y][f.x] = CAMINHO;
			f.x++;
			formigaAtual(m, f);
		}
		break;
	case SAIR:
		c.pausa = true;
		c.jogar = false;
		break;
	case PEGAR:
		// Pega ou deposita comida
		// Se alguma posição ao redor da formiga for armazém ela chama a função verificar armazém
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
		break;
	}
}

// Nessa função a matriz é percorrida e os números são substituidos
void desenharFrame(Mapa m, Controle c, Formiga f, Item it) {
	const int TILE = 40;
	const int PLACAR = 100;
	const char tempo[5] = { (int)c.tempoTotal };
	int x = (LARGURA / 2) - ((m.x * TILE) / 2), y = (PLACAR / 2) + (ALTURA / 2) - ((m.y * TILE) / 2);
	/*---------------------------- HUD -------------------------------*/
	al_draw_bitmap(c.imgPlaca, 0, 0, NULL);
	al_draw_text(c.fonte10, al_map_rgb(102, 51, 0), 550, 50, NULL, "  tempo ate o terremoto: ");
	al_draw_text(c.fonte10, al_map_rgb(102, 51, 0), 650, 50, NULL, tempo);
	//cout << "Tempo ate o terremoto: " << (int)c.tempoTotal << "   " << endl;
	/*----------------------------------------------------------------*/
	//al_draw_bitmap(m.imgBackground, 0, PLACAR, NULL);
	for(size_t i = 0; i < m.y; i++) {
		for(size_t j = 0; j < m.x; j++) {
			if(m.mapa[i][j] == PAREDE) {
				al_draw_bitmap(m.imgCaminho, x, y, NULL);
				al_draw_bitmap(m.imgParede, x, y, NULL);
			} else if(m.mapa[i][j] == CAMINHO) {
				al_draw_bitmap(m.imgCaminho, x, y, NULL);
				al_draw_bitmap(m.imgCaminho, x, y, NULL);
			} else if(m.mapa[i][j] == ITEM_PA) {
				al_draw_bitmap(m.imgCaminho, x, y, NULL);
				al_draw_bitmap(it.imgPa, x, y, NULL);
			} else if(m.mapa[i][j] == ITEM_BOTA) {
				al_draw_bitmap(m.imgCaminho, x, y, NULL);
				al_draw_bitmap(it.imgBota, x, y, NULL);
			} else if(m.mapa[i][j] == ITEM_TOCHA) {
				al_draw_bitmap(m.imgCaminho, x, y, NULL);
				al_draw_bitmap(it.imgTocha, x, y, NULL);
			} else if(m.mapa[i][j] == ITEM_KIT) {
				al_draw_bitmap(m.imgCaminho, x, y, NULL);
				al_draw_bitmap(it.imgKit, x, y, NULL);
			} else if(m.mapa[i][j] == ARMAZEM_MAPA_1) {
				al_draw_bitmap(m.imgCaminho, x, y, NULL);
				al_draw_bitmap(m.imgArmazem[DESMORONANDO], x, y, NULL);
			} else if(m.mapa[i][j] == ARMAZEM_MAPA_2) {
				al_draw_bitmap(m.imgCaminho, x, y, NULL);
				al_draw_bitmap(m.imgArmazem[NORMAL], x, y, NULL);
			} else if (m.mapa[i][j] == ARMAZEM_MAPA_3) {
				al_draw_bitmap(m.imgCaminho, x, y, NULL);
				al_draw_bitmap(m.imgArmazem[NORMAL], x, y, NULL);
			} else if(m.mapa[i][j] == FORMIGA_VAZIO) {
				al_draw_bitmap(m.imgCaminho, x, y, NULL);
				al_draw_bitmap(f.imgFormiga[f.DIRECAO], x, y, NULL);
			} else if(m.mapa[i][j] == FORMIGA_CHEIO) {
				al_draw_bitmap(m.imgCaminho, x, y, NULL);
				al_draw_bitmap(f.imgFormiga[f.DIRECAO], x, y, NULL);
				al_draw_bitmap(m.imgComida, x, y, NULL);
			}
			x += TILE;
		}
		y += TILE;
		x = (LARGURA / 2) - ((m.x * TILE) / 2);
	}
}

// Carrega o modo gráfico
void inicializar(Controle &c) {
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

//Carrega as imagens
void carregarRecursos(Mapa &m, Controle &c, Formiga &f, Item &i) {
	m.imgParede = al_load_bitmap("res/PAREDE.png");
	m.imgCaminho = al_load_bitmap("res/CAMINHO.png");
	m.imgBackground = al_load_bitmap("res/BACKGROUND.png");
	m.imgComida = al_load_bitmap("res/COMIDA.png");
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
	c.imgPlaca = al_load_bitmap("res/PLACA.png");
	c.imgMenu = al_load_bitmap("res/MENU.png");
	c.fonte10 = al_load_font("res/COMIC.ttf", 10, NULL);
}

// Verifica se o jogador venceu
bool venceu(int a[QUANTIDADE_ARMAZENS][QUANTIDADE_LOCAIS]) {
	for(size_t LOCAL = 0; LOCAL < 4; LOCAL++)
		if(a[ARMAZEM_1][LOCAL] == SEM_COMIDA)
			return true;
	return false;
}

// Tela de fim de jogo
void fimJogo(Controle c) {
	c.jogar = false;
	al_draw_bitmap(c.imgMenu, 0, 0, NULL);
	al_draw_text(c.fonte10, al_map_rgb(0, 0, 0), 50, 100, NULL, "PARABENS!");
	/*
	cout << "VOCE VENCEU O JOGO, PARABENS!!!\n\n";
	cout << "O tempo total foi: " << c.tempoExecucao << endl << endl;
	*/
	if (!al_is_event_queue_empty(c.fila_eventos)) {
		ALLEGRO_EVENT evento;
		al_wait_for_event(c.fila_eventos, &evento);
		if (evento.type == ALLEGRO_EVENT_KEY_CHAR) {
			switch (evento.keyboard.keycode)
			{
			case ALLEGRO_KEY_ENTER:
				c.reiniciar = true;
			}
		} else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			c.sair = true;
		}
	}
}

// Reinicia as variáveis
void reiniciar(Mapa m, Controle c, Formiga f, Item i) {
	c.jogar = true;
}
//
// FIM DA PARTE DE CÓDIGO ESCRITA POR CARLOS



/*---------------------------------------------------------------------------------------------------*/



// PARTE DO CÓDIGO ESCRITA POR MATEUS
//
// função para medir tempo e sortear novo mapa
void medirTempo(bool inicio, Mapa &m, Controle &c, Formiga &f) {
	if (inicio) {
		c.tempoInicial = clock();
	} else {
		c.tempoFinal = clock();
		c.tempoTotal -= (c.tempoFinal - c.tempoInicial) / (double)CLOCKS_PER_SEC;
		c.tempoExecucao -= (c.tempoFinal - c.tempoInicial) / (double)CLOCKS_PER_SEC;

		if (c.tempoTotal < 0) {
			novoMapa(m);
			posicionarFormiga(m, f);
			// FALTA CRIAR A FUNÇÃO QUE DISPÕE OS ARMAZÉNS NO MAPA (MATEUS)
			// FALTA CRIAR A FUNÇÃO QUE DISPÕE OS ITENS NO MAPA (CAIO) 
			// UM ITEM DE CADA JA TA LEGAL, E LEMBRE-SE QUE A PA PODE ACUMULAR COM O DO PRÓXIMO MAPA
			c.tempoTotal = TEMPO_MAXIMO;
		}
	}
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
	
	inicializar(c);
	carregarRecursos(m, c, f, i);
	novoMapa(m);
	posicionarFormiga(m, f);
	iniciarArmazem(m.armazem);

	/*------------------------------ LOOP PRINCIPAL ---------------------------*/

	while(!c.sair) {
		al_clear_to_color(al_map_rgb(0, 0, 0));

		if(c.menu) {
			menu(c);
		} else if (c.jogar) {
			medirTempo(true, m, c, f);
			desenharFrame(m, c, f, i);
			lerComandos(m, c, f);
			medirTempo(false, m, c, f);
		} else if(c.pausa) {

		}

		/*if(venceu(m.armazem)) {
			fimJogo(c);
			reiniciar(m, c, f, i);
		}*/

		al_flip_display();
	}

	/*-------------------------------------------------------------------------*/

	al_destroy_display(c.display);
	al_destroy_event_queue(c.fila_eventos);
	al_destroy_font(c.fonte10);

	return 0;
}
//
