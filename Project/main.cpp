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
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>
#include <list>
#include <time.h>
#include <conio.h>
#include <stdlib.h>
#include <vector>
//

// CONSTANTES
//
#define LARGURA 1000
#define ALTURA 640

#define TEMPO_MAXIMO 45.0

#define PAREDE 0
#define CAMINHO 1
#define ARMAZEM_MAPA_1 6
#define ARMAZEM_MAPA_2 7
#define ARMAZEM_MAPA_3 8
#define PERSONAGEM_VAZIO 9
#define PERSONAGEM_CHEIO 10

#define SEM_COMIDA 0
#define COMIDA_1 1
#define COMIDA_2 2
#define COMIDA_3 3
#define COMIDA_4 4
//

enum armazens { ARMAZEM_1, ARMAZEM_2, ARMAZEM_3 };
enum locais {POSICAO_1, POSICAO_2, POSICAO_3, POSICAO_4};
enum movimentos {SUBIR, DESCER, ESQUERDA, DIREITA, PEGAR, SAIR, NADA};

using namespace std;

/*-------------------------------- STRUCTS ------------------------------------*/

typedef struct Controle {
	bool menu = false, jogar = true, pausa = false, sair = false, facil = true;
	clock_t tempoInicial = 0, tempoFinal = 0;
	double tempoTotal = TEMPO_MAXIMO, tempoExecucao = 0;
	int mapaAtual = 0;
	ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
};

typedef struct Formiga {
	int comidaAtual = 0;
	bool vazio = true;
	int velocidade = 0;
	int x = 1, y = 1;
	ALLEGRO_BITMAP *fomiga;
};

typedef struct Mapa {
	int armazem[3][4];
	int **mapa;
	int x, y;
	ALLEGRO_BITMAP *parede;
	ALLEGRO_BITMAP *caminho;
};

/*-----------------------------------------------------------------------------*/




// PARTE DO CÓDIGO ESCRITA POR CAIO
//
// Apresenta a tela de menu ao jogador
void menu(Controle &c) {
	static bool opcao = false;
	
	//----------------- MENU --------------------- //

	//LÓGICA MENU

	//-------------------------------------------- //
}
//
// FIM DA PARTE DE CÓDIGO ESCRITA POR CAIO



/*---------------------------------------------------------------------------------------------------*/



// PARTE DO CÓDIGO ESCRITA POR CARLOS
//
// Verifica se a formiga está ou não com comida
void formigaAtual(Mapa &m, Formiga f) {
	if(f.vazio)
		m.mapa[f.x][f.y] = PERSONAGEM_VAZIO;
	else
		m.mapa[f.x][f.y] = PERSONAGEM_CHEIO;
}

// Inicializa os armazens
void iniciarArmazem(int a[3][4]) {
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 4; j++) {
			if(i == 0) {
				if (j == 0) {
					a[ARMAZEM_1][POSICAO_1] = COMIDA_4;
					a[ARMAZEM_1][POSICAO_2] = COMIDA_3;
					a[ARMAZEM_1][POSICAO_3] = COMIDA_2;
					a[ARMAZEM_1][POSICAO_3] = COMIDA_1;
				}
			} else {
				a[i][j] = SEM_COMIDA;
			}
		}
	}
}

// Cria um novo mapa
void novoMapa(Mapa &m) {
	list <pair <int, int> > drillers;

	srand(time(0));

	m.x = 30 + (rand() % 20);
	m.y = (m.x / 3) * 2;
	m.mapa = new int *[m.y];
	for(size_t y = 0; y < m.y; y++)
		m.mapa[y] = new int[m.x];

	for(size_t x = 0; x < m.x; x++)
		for(size_t y = 0; y < m.y; y++)
			m.mapa[y][x] = PAREDE;

	drillers.push_back(make_pair(m.x / 2, m.y / 2));

	while(drillers.size() > 0) {
		list <pair <int, int> >::iterator n, _n, temp;
		n = drillers.begin();
		_n = drillers.end();
		while(n != _n) {
			bool remove_driller = false;
			switch(rand() % 4)
			{
			case 0:
				(*n).second -= 2;
				if((*n).second < 0 || m.mapa[(*n).second][(*n).first]) {
					remove_driller = true;
					break;
				}
				m.mapa[(*n).second + 1][(*n).first] = CAMINHO;
				break;
			case 1:
				(*n).second += 2;
				if((*n).second >= m.y || m.mapa[(*n).second][(*n).first]) {
					remove_driller = true;
					break;
				}
				m.mapa[(*n).second - 1][(*n).first] = CAMINHO;
				break;
			case 2:
				(*n).first -= 2;
				if((*n).first < 0 || m.mapa[(*n).second][(*n).first]) {
					remove_driller = true;
					break;
				}
				m.mapa[(*n).second][(*n).first + 1] = CAMINHO;
				break;
			case 3:
				(*n).first += 2;
				if((*n).first >= m.x || m.mapa[(*n).second][(*n).first]) {
					remove_driller = true;
					break;
				}
				m.mapa[(*n).second][(*n).first - 1] = CAMINHO;
				break;
			}
			if(remove_driller) {
				n = drillers.erase(n);
			} else {
				drillers.push_back(make_pair((*n).first, (*n).second));
				if(rand() % 2)
					drillers.push_back(make_pair((*n).first, (*n).second));
				m.mapa[(*n).second][(*n).first] = CAMINHO;
				++n;
			}
		}
	}
}

// Nessa função verifica-se se vai retirar ou colocar a comida no armazém
void verificarArmazem(Formiga &f, Mapa &m, int ARMAZEM) {
	int LOCAL;
	bool retirar = false;

	if(f.comidaAtual == 0) { // se a formiga não tiver comida, significa que ela vai pegar do armazém
		for (LOCAL = 3; LOCAL >= 0; LOCAL--) {
			if (m.armazem[ARMAZEM][LOCAL] != SEM_COMIDA) {
				f.comidaAtual = m.armazem[ARMAZEM][LOCAL];
				m.armazem[ARMAZEM][LOCAL] = SEM_COMIDA;
				m.mapa[f.x][f.y] = PERSONAGEM_CHEIO;
				f.vazio = false;
				LOCAL = -1;
			}
		}
	} else { // se já tiver ela vai colocar no armazém
		for(LOCAL = 0; LOCAL <= 3; LOCAL++) {
			if(m.armazem[ARMAZEM][LOCAL] == SEM_COMIDA) {
				if(LOCAL == 0)
					retirar = true;
				else if(f.comidaAtual < m.armazem[ARMAZEM][LOCAL - 1])
					retirar = true;

				if(retirar) {
					m.armazem[ARMAZEM][LOCAL] = f.comidaAtual;
					f.comidaAtual = SEM_COMIDA;
					m.mapa[f.x][f.y] = PERSONAGEM_VAZIO;
					f.vazio = true;
				}
				LOCAL = 4;
			}
		}
	}
}

// Nessa função são executados os comandos do jogo
void lerComandos(Mapa &m, Controle &c, Formiga &f) {
	 ALLEGRO_EVENT evento;
	 int tecla = NADA;

    al_wait_for_event(c.fila_eventos, &evento);
 
    if (evento.type == ALLEGRO_EVENT_KEY_DOWN){
        switch(evento.keyboard.keycode)
		{
        case ALLEGRO_KEY_UP:
            tecla = SUBIR;
            break;
        case ALLEGRO_KEY_DOWN:
            tecla = DESCER;
            break;
        case ALLEGRO_KEY_LEFT:
            tecla = ESQUERDA;
            break;
        case ALLEGRO_KEY_RIGHT:
            tecla = DIREITA;
            break;
        case ALLEGRO_KEY_ESCAPE:
            tecla = SAIR;
			break;
		case ALLEGRO_KEY_SPACE:
			tecla = PEGAR;
        }
    }
    else if (evento.type == ALLEGRO_EVENT_KEY_UP){
        tecla = NADA;
    }
    else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
        c.sair = true;
    }

	switch(tecla)
	{
	case SUBIR:
		if(m.mapa[f.x - 1][f.y] == CAMINHO) {
			m.mapa[f.x][f.y] = CAMINHO;
			f.x--;
			formigaAtual(m, f);
		}
		break;
	case DESCER:
		if(m.mapa[f.x + 1][f.y] == CAMINHO) {
			m.mapa[f.x][f.y] = CAMINHO;
			f.x++;
			formigaAtual(m, f);
		}
		break;
	case ESQUERDA:
		if(m.mapa[f.x][f.y - 1] == CAMINHO) {
			m.mapa[f.x][f.y] = CAMINHO;
			f.y--;
			formigaAtual(m, f);
		}
		break;
	case DIREITA:
		if(m.mapa[f.x][f.y + 1] == CAMINHO) {
			m.mapa[f.x][f.y] = CAMINHO;
			f.y++;
			formigaAtual(m, f);
		}
		break;
	case SAIR:
		c.pausa = true;
		c.jogar = false;
		break;
	case PEGAR:  // pega ou deposita comida
	// se alguma posição ao redor da formiga for comida ela pega
		if (m.mapa[f.x + 1][f.y] == ARMAZEM_MAPA_1 ||
			m.mapa[f.x - 1][f.y] == ARMAZEM_MAPA_1 ||
			m.mapa[f.x][f.y + 1] == ARMAZEM_MAPA_1 ||
			m.mapa[f.x][f.y - 1] == ARMAZEM_MAPA_1) // armazem 1
			verificarArmazem(f, m, ARMAZEM_1);
		if (m.mapa[f.x + 1][f.y] == ARMAZEM_MAPA_2 ||
			m.mapa[f.x - 1][f.y] == ARMAZEM_MAPA_2 ||
			m.mapa[f.x][f.y + 1] == ARMAZEM_MAPA_2 ||
			m.mapa[f.x][f.y - 1] == ARMAZEM_MAPA_2) // armazem 2 (meio)
			verificarArmazem(f, m, ARMAZEM_2);
		if (m.mapa[f.x + 1][f.y] == ARMAZEM_MAPA_3 ||
			m.mapa[f.x - 1][f.y] == ARMAZEM_MAPA_3 ||
			m.mapa[f.x][f.y + 1] == ARMAZEM_MAPA_3 ||
			m.mapa[f.x][f.y - 1] == ARMAZEM_MAPA_3) // armazem 3 (cima final)
			verificarArmazem(f, m, ARMAZEM_3);
		break;
	}
	al_rest(f.velocidade);
}

// Nessa função a matriz é percorrida e os números são substituidos
void imprimir(Mapa m, Controle c, Formiga f) {
	int x = 0, y = 0;
	int TILE = 20;
	/*---------------------------- HUD -------------------------------*/
	//cout << "Tempo ate o terremoto: " << (int)c.tempoTotal << "   " << endl;

	for(int i = 0; i < m.y; i++) {
		for(int j = 0; j < m.x; j++) {
			if(m.mapa[i][j] == PAREDE) {
				al_draw_bitmap(m.caminho, x, y, 0);
				al_draw_bitmap(m.parede, x, y, 0);
			} else if (m.mapa[i][j] == CAMINHO) {
				al_draw_bitmap(m.caminho, x, y, 0);
			} else if (m.mapa[i][j] == PERSONAGEM_VAZIO) {

			} else if (m.mapa[i][j] == PERSONAGEM_CHEIO) {

			} else if (m.mapa[i][j] == ARMAZEM_MAPA_1) {

			} else if (m.mapa[i][j] == ARMAZEM_MAPA_2) {

			} else if (m.mapa[i][j] == ARMAZEM_MAPA_3) {

			}
			x += TILE;
		}
		y += TILE;
		x = 0;
	}
}

// Verificam se o jogador venceu
bool venceuDificil(int a[3][4]) {
	for(int i = 0; i < 4; i++)
		if(a[ARMAZEM_3][i] == SEM_COMIDA)
			return false;
	return true;
}

bool venceuFacil(int a[3][4]) {
	if(a[ARMAZEM_3][POSICAO_1] == COMIDA_4)
		return true;
	return false;
}
//-------------------------------
// Tela de fim de jogo
bool venceuJogo(double tempo) {
	/*
	system("cls");
	cout << "VOCE VENCEU O JOGO, PARABENS!!!\n\n";
	cout << "O tempo total foi: " << tempo << endl << endl;
	system("pause");
	*/
	return true;
}
//-------------------------------
//
// FIM DA PARTE DE CÓDIGO ESCRITA POR CARLOS



/*---------------------------------------------------------------------------------------------------*/



// PARTE DO CÓDIGO ESCRITA POR MATEUS
//
// Muda a posição da formiga para não ser soterrada
void mudarPosicao(Formiga &f, Mapa &m, Controle c, int PESONAGEM) {
	if(m.mapa[f.x][f.y] == PAREDE) {
		m.mapa[f.x][f.y] = PESONAGEM;
	} else {
		for (int i = 1; i < 50; i++) {
			if (m.mapa[f.x + i][f.y] == PAREDE) {
				m.mapa[f.x + i][f.y] = PESONAGEM;
				f.x++;
			} else if (m.mapa[f.x - i][f.y] == PAREDE) {
				m.mapa[f.x - i][f.y] = PESONAGEM;
				f.x--;
			} else if (m.mapa[f.x][f.y + i] == PAREDE) {
				m.mapa[f.x][f.y + i] = PESONAGEM;
				f.y++;
			} else if (m.mapa[f.x][f.y - i] == PAREDE) {
				m.mapa[f.x][f.y - i] = PESONAGEM;
				f.y--;
			}
		}
	}
}
//
// função para medir tempo e sortear novo mapa
void medirTempo(bool inicio, Mapa &m, Controle &c, Formiga &f) {
	if (inicio) {
		c.tempoInicial = clock();
	} else {
		int aux = c.mapaAtual;

		c.tempoFinal = clock();
		c.tempoTotal -= (c.tempoFinal - c.tempoInicial) / (double)CLOCKS_PER_SEC;
		c.tempoExecucao -= (c.tempoFinal - c.tempoInicial) / (double)CLOCKS_PER_SEC;

		if (c.tempoTotal < 0) {
			for (int i = 0; i < m.x; i++) {
				for (int j = 0; j < m.y; j++) {
					if (m.mapa[i][j] == PERSONAGEM_VAZIO)
						m.mapa[i][j] = CAMINHO;
					if (m.mapa[i][j] == PERSONAGEM_CHEIO)
						m.mapa[i][j] = CAMINHO;
				}
			}

			srand(time(NULL));

			while (c.mapaAtual == aux)
				c.mapaAtual = rand() % 3;

			c.tempoTotal = TEMPO_MAXIMO;

			if (f.vazio)
				mudarPosicao(f, m, c, PERSONAGEM_VAZIO);
			else
				mudarPosicao(f, m, c, PERSONAGEM_CHEIO);
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
	ALLEGRO_DISPLAY *display = NULL;
	/*-------------------------------------------------------------------------*/

	/*------------------------------ INICIALIZAÇÃO ----------------------------*/
	// Inicializando gráficos
	al_init();
	al_set_new_display_flags(ALLEGRO_OPENGL);
	display = al_create_display(LARGURA, ALTURA);
	al_set_window_title(display, "The Ant Quest");
	al_init_image_addon();
	al_install_keyboard();
	c.fila_eventos = al_create_event_queue();
	al_register_event_source(c.fila_eventos, al_get_keyboard_event_source());
	al_register_event_source(c.fila_eventos, al_get_display_event_source(display));

	// Carregando imagens
	m.parede = al_load_bitmap("PAREDE.png");
	m.caminho = al_load_bitmap("CAMINHO.png");
	f.fomiga = al_load_bitmap("FORMIGA.png");

	// Inicialização dos conteúdos
	novoMapa(m);
	iniciarArmazem(m.armazem);

	/*------------------------------ LOOP PRINCIPAL ---------------------------*/
	while(!c.sair) {
		al_clear_to_color(al_map_rgb(0, 0, 0));

		if(c.menu) {
			menu(c);
		} else if (c.jogar) {
			//medirTempo(true, m, c, f);
			imprimir(m, c, f);
			//lerComandos(m, c, f);
			//medirTempo(false, m, c, f);
		} else if(c.pausa) {

		}

		if(c.facil && venceuFacil(m.armazem)) {
			c.sair = venceuJogo(c.tempoExecucao);
		} else if(!c.facil && venceuDificil(m.armazem)) {
			c.sair = venceuJogo(c.tempoExecucao);
		}

		al_flip_display();
	}
	/*-------------------------------------------------------------------------*/
	al_destroy_display(display);
	al_destroy_event_queue(c.fila_eventos);
	return 0;
}
//
