#pragma once

// BIBLIOTECAS
//
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>
#include <list>
#include <time.h>
#include <string>
//
#include "menu.h"
//

// Inicializa todas as variaveis do programa e atribui seus valores (CARLOS)
void inicializarVariaveis(Mapa& m, Controle& c, Formiga& f, Item& i) {
	c.opcao = NULL;
	c.menu = true;
	c.jogar = false;
	c.pausa = false;
	c.sair = false;
	c.venceu = true;
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

//Carrega os recursos do jogo (CARLOS)
void carregarRecursos(Mapa& m, Controle& c, Formiga& f, Item& i) {
	int INTERVALO = 10;
	c.somTema = al_load_sample("res/audio/TEMA.ogg");
	c.somCavar = al_load_sample("res/audio/PA.ogg");
	c.somTerremoto = al_load_sample("res/audio/TERREMOTO.ogg");
	c.somVitoria = al_load_sample("res/audio/VITORIA.ogg");
	c.somKit = al_load_sample("res/audio/KIT.ogg");
	c.somSelecionar = al_load_sample("res/audio/SELECIONAR.ogg");
	c.somEnter = al_load_sample("res/audio/ENTER.ogg");
	c.somTocha = al_load_sample("res/audio/TOCHA.ogg");
	c.somPa = al_load_sample("res/audio/CAVAR.ogg");
	c.somCaminhada = al_load_sample("res/audio/CAMINHADA.ogg");
	c.somCouro = al_load_sample("res/audio/COURO.ogg");
	c.somFolha = al_load_sample("res/audio/FOLHA.ogg");
	c.imgPlaca = al_load_bitmap("res/img/PLACA.png");
	c.imgMenu = al_load_bitmap("res/img/MENU.png");
	c.imgBota = al_load_bitmap("res/img/ITEM_BOTA_HUD.png");
	c.imgPa = al_load_bitmap("res/img/ITEM_PA_HUD.png");
	c.imgTocha = al_load_bitmap("res/img/ITEM_TOCHA_HUD.png");
	c.imgRelogio = al_load_bitmap("res/img/RELOGIO_HUD.png");
	c.imgSelecao[SELECIONADO] = al_load_bitmap("res/img/OPCAO_CLARO.png");
	c.imgSelecao[NAO_SELECIONADO] = al_load_bitmap("res/img/OPCAO_ESCURO.png");
	c.imgRecordes = al_load_bitmap("res/img/RECORDES.png");
	for (size_t TAM = 0; TAM < TOTAL_FONTES; TAM++) {
		c.fonte[TAM] = al_load_font("res/fonte/COMIC.ttf", INTERVALO, NULL);
		INTERVALO += 10;
	}
	m.imgParede = al_load_bitmap("res/img/PAREDE.png");
	m.imgCaminho = al_load_bitmap("res/img/CAMINHO.png");
	m.imgBackground = al_load_bitmap("res/img/BACKGROUND.png");
	m.imgComida[SUBIR] = al_load_bitmap("res/img/COMIDA_CIMA.png");
	m.imgComida[DESCER] = al_load_bitmap("res/img/COMIDA_BAIXO.png");
	m.imgComida[ESQUERDA] = al_load_bitmap("res/img/COMIDA_ESQUERDA.png");
	m.imgComida[DIREITA] = al_load_bitmap("res/img/COMIDA_DIREITA.png");
	m.imgSombra[GRANDE] = al_load_bitmap("res/img/SOMBRA_GRANDE.png");
	m.imgSombra[PEQUENA] = al_load_bitmap("res/img/SOMBRA_PEQUENA.png");
	m.imgArmazem[NORMAL] = al_load_bitmap("res/img/ARMAZEM.png");
	m.imgArmazem[DESMORONANDO] = al_load_bitmap("res/img/ARMAZEM_DESMORONANDO.png");
	m.imgComidaAmazem[COMIDA_1] = al_load_bitmap("res/img/COMIDA_1.png");
	m.imgComidaAmazem[COMIDA_2] = al_load_bitmap("res/img/COMIDA_2.png");
	m.imgComidaAmazem[COMIDA_3] = al_load_bitmap("res/img/COMIDA_3.png");
	m.imgComidaAmazem[COMIDA_4] = al_load_bitmap("res/img/COMIDA_4.png");
	f.imgFormiga[SUBIR] = al_load_bitmap("res/img/FORMIGA_CIMA.png");
	f.imgFormiga[DESCER] = al_load_bitmap("res/img/FORMIGA_BAIXO.png");
	f.imgFormiga[ESQUERDA] = al_load_bitmap("res/img/FORMIGA_ESQUERDA.png");
	f.imgFormiga[DIREITA] = al_load_bitmap("res/img/FORMIGA_DIREITA.png");
	i.imgBota = al_load_bitmap("res/img/ITEM_BOTA.png");
	i.imgKit = al_load_bitmap("res/img/ITEM_KIT.png");
	i.imgPa = al_load_bitmap("res/img/ITEM_PA.png");
	i.imgTocha = al_load_bitmap("res/img/ITEM_TOCHA.png");
}

// Finaliza o programa, destruindo todos os objetos criados (CARLOS)
void finalizar(Mapa& m, Controle& c, Formiga& f, Item& i) {
	al_destroy_display(c.display);
	al_destroy_event_queue(c.fila_eventos);
	al_destroy_sample(c.somTema);
	al_destroy_sample(c.somVitoria);
	al_destroy_sample(c.somTerremoto);
	al_destroy_sample(c.somPa);
	al_destroy_sample(c.somKit);
	al_destroy_sample(c.somSelecionar);
	al_destroy_sample(c.somEnter);
	al_destroy_sample(c.somTocha);
	al_destroy_sample(c.somCavar);
	al_destroy_sample(c.somCaminhada);
	al_destroy_sample(c.somCouro);
	al_destroy_sample(c.somFolha);
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
	al_destroy_bitmap(m.imgArmazem[NORMAL]);
	al_destroy_bitmap(m.imgArmazem[DESMORONANDO]);
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

// Inicia o tempo dos itens (CARLOS E CAIO)
void tempoItem(bool inicio, Item& i) {
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
void posicionarItens(Mapa& m, Item& i) {
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

// Gera um novo mapa (CARLOS)(CÓDIDO TIRADO DO EXEMPLO APRESENTADO PELO PROFESSOR E ADAPTADO PARA NOSSO JOGO)
void novoMapa(Mapa& m) {
	std::list <std::pair <int, int> > drillers;
	int** map = NULL;
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
		map = new int* [m.y];

		for (size_t y = 0; y < m.y; y++)
			map[y] = new int[m.x];

		for (size_t x = 0; x < m.x; x++)
			for (size_t y = 0; y < m.y; y++)
				map[y][x] = PAREDE;

		drillers.push_back(std::make_pair(m.x / 2, m.y / 2));

		while (drillers.size() > 0) {
			std::list <std::pair <int, int> >::iterator n, _n, temp;
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
					drillers.push_back(std::make_pair((*n).first, (*n).second));
					if (rand() % 2)
						drillers.push_back(std::make_pair((*n).first, (*n).second));
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

	m.mapa = new int* [m.y];

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
void posicionarFormiga(Mapa& m, Formiga& f) {
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
void verificarArmazem(Formiga& f, Mapa& m, int ARMAZEM) {
	int LOCAL;
	bool retirar = false;

	if (f.comidaAtual == SEM_COMIDA) { // Se a formiga não tiver segurando comida, significa que ela vai pegar do armazém
		for (LOCAL = LOCAL_4; LOCAL >= LOCAL_1; LOCAL--) {
			if (m.armazem[ARMAZEM][LOCAL] != SEM_COMIDA) {
				f.comidaAtual = m.armazem[ARMAZEM][LOCAL];
				m.armazem[ARMAZEM][LOCAL] = SEM_COMIDA;
				m.mapa[f.y][f.x] = FORMIGA_CHEIO;
				f.vazio = false;
				break;
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
				break;
			}
		}
	}
}

// Nessa função são executados os comandos do jogo (CARLOS, CAIO E MATEUS)
// Obs.: Todos os comentários dessa função servem para todas as posições pressionadas, apenas para redução de linhas
void lerComandos(Mapa& m, Controle& c, Formiga& f, Item& i) {
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
						// Verifica se nessa posição existe uma PÁ
						if (m.mapa[f.y - 1][f.x] == ITEM_PA) {
							// Ao pegar o item PÁ, acrescenta mais um no contador
							tocarSample(c.somPa, c.sfx);
							i.quantidadePa++;
						}
						// Verifica se nessa posição existe uma BOTA
						if (m.mapa[f.y - 1][f.x] == ITEM_BOTA) {
							// Caso ele já obtiver a BOTA, acrescenta 30 segundos
							tocarSample(c.somCouro, c.sfx);
							if (i.bota) {
								i.tempoBota += 30.0;
							}
							// Ao pegar o item BOTA, inicia a função do item
							else {
								i.bota = true;
								i.tempoBota = 30.0;
							}
						}
						// Verifica se nessa posição existe uma TOCHA.
						if (m.mapa[f.y - 1][f.x] == ITEM_TOCHA) {
							// Caso ele já obtiver a TOCHA, acrescenta 30 segundos
							tocarSample(c.somTocha, c.sfx);
							if (i.tocha) {
								i.tempoTocha += 30.0;
							}
							// Ao pegar o item TOCHA, inicia a função do item
							else {
								i.tocha = true;
								i.tempoTocha = 30.0;
							}
						}
						// Verifica se nessa posição existe um KIT DE REPAROS
						if (m.mapa[f.y - 1][f.x] == ITEM_KIT) {
							// Inicia a função do item, acrescenta 30 segundos no tempo até o próximo terremoto
							tocarSample(c.somKit, c.sfx);
							c.tempoTotal += 30;
						}
						tocarSample(c.somCaminhada, c.sfx);
						m.mapa[f.y][f.x] = CAMINHO;
						f.y--;
						formigaAtual(m, f);
					}
					// Contabiliza o tempo total do jogo
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
							tocarSample(c.somPa, c.sfx);
							i.quantidadePa++;
						}
						if (m.mapa[f.y + 1][f.x] == ITEM_BOTA) {
							tocarSample(c.somCouro, c.sfx);
							if (i.bota) {
								i.tempoBota += 30.0;
							}
							else {
								i.bota = true;
								i.tempoBota = 30.0;
							}
						}
						if (m.mapa[f.y + 1][f.x] == ITEM_TOCHA) {
							tocarSample(c.somTocha, c.sfx);
							if (i.tocha) {
								i.tempoTocha += 30.0;
							}
							else {
								i.tocha = true;
								i.tempoTocha = 30.0;
							}
						}
						if (m.mapa[f.y + 1][f.x] == ITEM_KIT) {
							tocarSample(c.somKit, c.sfx);
							c.tempoTotal += 30;
						}
						tocarSample(c.somCaminhada, c.sfx);
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
							tocarSample(c.somPa, c.sfx);
							i.quantidadePa++;
						}
						if (m.mapa[f.y][f.x - 1] == ITEM_BOTA) {
							tocarSample(c.somCouro, c.sfx);
							if (i.bota) {
								i.tempoBota += 30.0;
							}
							else {
								i.bota = true;
								i.tempoBota = 30.0;
							}
						}
						if (m.mapa[f.y][f.x - 1] == ITEM_TOCHA) {
							tocarSample(c.somTocha, c.sfx);
							if (i.tocha) {
								i.tempoTocha += 30.0;
							}
							else {
								i.tocha = true;
								i.tempoTocha = 30.0;
							}
						}
						if (m.mapa[f.y][f.x - 1] == ITEM_KIT) {
							tocarSample(c.somKit, c.sfx);
							c.tempoTotal += 30;
						}
						tocarSample(c.somCaminhada, c.sfx);
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
							tocarSample(c.somPa, c.sfx);
							i.quantidadePa++;
						}
						if (m.mapa[f.y][f.x + 1] == ITEM_BOTA) {
							tocarSample(c.somCouro, c.sfx);
							if (i.bota) {
								i.tempoBota += 30.0;
							}
							else {
								i.bota = true;
								i.tempoBota = 30.0;
							}
						}
						if (m.mapa[f.y][f.x + 1] == ITEM_TOCHA) {
							tocarSample(c.somTocha, c.sfx);
							if (i.tocha) {
								i.tempoTocha += 30.0;
							}
							else {
								i.tocha = true;
								i.tempoTocha = 30.0;
							}
						}
						if (m.mapa[f.y][f.x + 1] == ITEM_KIT) {
							tocarSample(c.somKit, c.sfx);
							c.tempoTotal += 30;
						}
						tocarSample(c.somCaminhada, c.sfx);
						m.mapa[f.y][f.x] = CAMINHO;
						f.x++;
						formigaAtual(m, f);
					}
					f.tempoVelocidade = clock();
				}
				f.DIRECAO = DIREITA;
				break;
			case ALLEGRO_KEY_ESCAPE:
				tocarSample(c.somEnter, c.sfx);
				c.pausa = true;
				c.jogar = false;
				break;
			case ALLEGRO_KEY_ENTER:
			case ALLEGRO_KEY_SPACE:
				// Pega ou deposita comida
				// Se alguma posição ao redor da formiga for armazém ela chama a função verificar armazém
				if (m.mapa[f.y + 1][f.x] == ARMAZEM_MAPA_1 && f.DIRECAO == DESCER ||
					m.mapa[f.y - 1][f.x] == ARMAZEM_MAPA_1 && f.DIRECAO == SUBIR ||
					m.mapa[f.y][f.x + 1] == ARMAZEM_MAPA_1 && f.DIRECAO == DIREITA ||
					m.mapa[f.y][f.x - 1] == ARMAZEM_MAPA_1 && f.DIRECAO == ESQUERDA) {		// O ARMAZÉM 1 É O INICIAL (DESMORONANDO)
					tocarSample(c.somFolha, c.sfx);
					verificarArmazem(f, m, ARMAZEM_1);
				}
				if (m.mapa[f.y + 1][f.x] == ARMAZEM_MAPA_2 && f.DIRECAO == DESCER ||
					m.mapa[f.y - 1][f.x] == ARMAZEM_MAPA_2 && f.DIRECAO == SUBIR ||
					m.mapa[f.y][f.x + 1] == ARMAZEM_MAPA_2 && f.DIRECAO == DIREITA ||
					m.mapa[f.y][f.x - 1] == ARMAZEM_MAPA_2 && f.DIRECAO == ESQUERDA) {
					tocarSample(c.somFolha, c.sfx);
					verificarArmazem(f, m, ARMAZEM_2);
				}
				if (m.mapa[f.y + 1][f.x] == ARMAZEM_MAPA_3 && f.DIRECAO == DESCER ||
					m.mapa[f.y - 1][f.x] == ARMAZEM_MAPA_3 && f.DIRECAO == SUBIR ||
					m.mapa[f.y][f.x + 1] == ARMAZEM_MAPA_3 && f.DIRECAO == DIREITA ||
					m.mapa[f.y][f.x - 1] == ARMAZEM_MAPA_3 && f.DIRECAO == ESQUERDA) {
					tocarSample(c.somFolha, c.sfx);
					verificarArmazem(f, m, ARMAZEM_3);
				}
				// Verifica se a frente da formiga existe parede, se sim, utiliza o item PÁ destruindo a parede selecionada
				if (m.mapa[f.y - 1][f.x] == PAREDE && f.DIRECAO == SUBIR && i.quantidadePa > 0) {
					tocarSample(c.somCavar, c.sfx);
					m.mapa[f.y - 1][f.x] = CAMINHO;
					// Diminui a quantidade de PÁS restantes
					i.quantidadePa--;
				}
				if (m.mapa[f.y + 1][f.x] == PAREDE && f.DIRECAO == DESCER && i.quantidadePa > 0) {
					tocarSample(c.somCavar, c.sfx);
					m.mapa[f.y + 1][f.x] = CAMINHO;
					i.quantidadePa--;
				}
				if (m.mapa[f.y][f.x + 1] == PAREDE && f.DIRECAO == DIREITA && i.quantidadePa > 0) {
					tocarSample(c.somCavar, c.sfx);
					m.mapa[f.y][f.x + 1] = CAMINHO;
					i.quantidadePa--;
				}
				if (m.mapa[f.y][f.x - 1] == PAREDE && f.DIRECAO == ESQUERDA && i.quantidadePa > 0) {
					tocarSample(c.somCavar, c.sfx);
					m.mapa[f.y][f.x - 1] = CAMINHO;
					i.quantidadePa--;
				}
			}
		}
	}
	// Inicia a função do item BOTA, aumentando a velocidade máxima
	if (i.bota) {
		f.velocidade = 100;
	}
	// Velocidade padrão
	else {
		f.velocidade = 200;
	}
	// Desabilita o efeito do item BOTA após o término do tempo
	if (i.tempoBota < 0.0) {
		i.bota = false;
	}
	// Desabilita o efeito do item TOCHA após o término do tempo
	if (i.tempoTocha < 0.0) {
		i.tocha = false;
	}
}

// Nessa função a matriz é percorrida e os números são substituidos (CARLOS)
void desenharFrame(Mapa m, Controle c, Formiga f, Item it) {
	const int TILE = 40;
	const int PLACAR = 100;
	std::string tempoString = std::to_string((int)c.tempoTotal);
	char const* tempoChar = tempoString.c_str();
	std::string tempoStringTocha = std::to_string((int)it.tempoTocha);
	char const* tempoTocha = tempoStringTocha.c_str();
	std::string tempoStringBota = std::to_string((int)it.tempoBota);
	char const* tempoBota = tempoStringBota.c_str();
	std::string paString = std::to_string((int)it.quantidadePa);
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
				al_draw_bitmap(m.imgArmazem[DESMORONANDO], x, y, NULL);
				// Renderiza a pilha de comidas dentro do armazém
				if (m.armazem[ARMAZEM_1][LOCAL_1] == COMIDA_1)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_1], x, y + 19, NULL);
				else if (m.armazem[ARMAZEM_1][LOCAL_1] == COMIDA_2)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_2], x, y + 12, NULL);
				else if (m.armazem[ARMAZEM_1][LOCAL_1] == COMIDA_3)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_3], x, y + 6, NULL);
				else if (m.armazem[ARMAZEM_1][LOCAL_1] == COMIDA_4)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_4], x, y, NULL); // COMIDA PADRAO CAMADA

				if (m.armazem[ARMAZEM_1][LOCAL_2] == COMIDA_1)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_1], x, y + 13, NULL);
				else if (m.armazem[ARMAZEM_1][LOCAL_2] == COMIDA_2)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_2], x, y + 6, NULL);
				else if (m.armazem[ARMAZEM_1][LOCAL_2] == COMIDA_3)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_3], x, y, NULL); // COMIDA PADRAO CAMADA
				else if (m.armazem[ARMAZEM_1][LOCAL_2] == COMIDA_4)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_4], x, y - 6, NULL);

				if (m.armazem[ARMAZEM_1][LOCAL_3] == COMIDA_1)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_1], x, y + 7, NULL);
				else if (m.armazem[ARMAZEM_1][LOCAL_3] == COMIDA_2)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_2], x, y, NULL); // COMIDA PADRAO CAMADA
				else if (m.armazem[ARMAZEM_1][LOCAL_3] == COMIDA_3)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_3], x, y - 6, NULL);
				else if (m.armazem[ARMAZEM_1][LOCAL_3] == COMIDA_4)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_4], x, y - 12, NULL);

				if (m.armazem[ARMAZEM_1][LOCAL_4] == COMIDA_1)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_1], x, y, NULL); // COMIDA PADRAO CAMADA
				else if (m.armazem[ARMAZEM_1][LOCAL_4] == COMIDA_2)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_2], x, y - 7, NULL);
				else if (m.armazem[ARMAZEM_1][LOCAL_4] == COMIDA_3)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_3], x, y - 13, NULL);
				else if (m.armazem[ARMAZEM_1][LOCAL_4] == COMIDA_4)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_4], x, y - 19, NULL);
			}
			else if (m.mapa[i][j] == ARMAZEM_MAPA_2) {
				al_draw_bitmap(m.imgCaminho, x, y, NULL);
				al_draw_bitmap(m.imgArmazem[NORMAL], x, y, NULL);
				if (m.armazem[ARMAZEM_2][LOCAL_1] == COMIDA_1)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_1], x, y + 19, NULL);
				else if (m.armazem[ARMAZEM_2][LOCAL_1] == COMIDA_2)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_2], x, y + 12, NULL);
				else if (m.armazem[ARMAZEM_2][LOCAL_1] == COMIDA_3)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_3], x, y + 6, NULL);
				else if (m.armazem[ARMAZEM_2][LOCAL_1] == COMIDA_4)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_4], x, y, NULL);

				if (m.armazem[ARMAZEM_2][LOCAL_2] == COMIDA_1)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_1], x, y + 13, NULL);
				else if (m.armazem[ARMAZEM_2][LOCAL_2] == COMIDA_2)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_2], x, y + 6, NULL);
				else if (m.armazem[ARMAZEM_2][LOCAL_2] == COMIDA_3)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_3], x, y, NULL);
				else if (m.armazem[ARMAZEM_2][LOCAL_2] == COMIDA_4)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_4], x, y - 6, NULL);

				if (m.armazem[ARMAZEM_2][LOCAL_3] == COMIDA_1)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_1], x, y + 7, NULL);
				else if (m.armazem[ARMAZEM_2][LOCAL_3] == COMIDA_2)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_2], x, y, NULL);
				else if (m.armazem[ARMAZEM_2][LOCAL_3] == COMIDA_3)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_3], x, y - 6, NULL);
				else if (m.armazem[ARMAZEM_2][LOCAL_3] == COMIDA_4)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_4], x, y - 12, NULL);

				if (m.armazem[ARMAZEM_2][LOCAL_4] == COMIDA_1)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_1], x, y, NULL);
				else if (m.armazem[ARMAZEM_2][LOCAL_4] == COMIDA_2)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_2], x, y - 6, NULL);
				else if (m.armazem[ARMAZEM_2][LOCAL_4] == COMIDA_3)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_3], x, y - 12, NULL);
				else if (m.armazem[ARMAZEM_2][LOCAL_4] == COMIDA_4)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_4], x, y - 19, NULL);
			}
			else if (m.mapa[i][j] == ARMAZEM_MAPA_3) {
				al_draw_bitmap(m.imgCaminho, x, y, NULL);
				al_draw_bitmap(m.imgArmazem[NORMAL], x, y, NULL);
				if (m.armazem[ARMAZEM_3][LOCAL_1] == COMIDA_1)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_1], x, y + 19, NULL);
				else if (m.armazem[ARMAZEM_3][LOCAL_1] == COMIDA_2)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_2], x, y + 12, NULL);
				else if (m.armazem[ARMAZEM_3][LOCAL_1] == COMIDA_3)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_3], x, y + 6, NULL);
				else if (m.armazem[ARMAZEM_3][LOCAL_1] == COMIDA_4)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_4], x, y, NULL);

				if (m.armazem[ARMAZEM_3][LOCAL_2] == COMIDA_1)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_1], x, y + 13, NULL);
				else if (m.armazem[ARMAZEM_3][LOCAL_2] == COMIDA_2)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_2], x, y + 6, NULL);
				else if (m.armazem[ARMAZEM_3][LOCAL_2] == COMIDA_3)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_3], x, y, NULL);
				else if (m.armazem[ARMAZEM_3][LOCAL_2] == COMIDA_4)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_4], x, y - 6, NULL);

				if (m.armazem[ARMAZEM_3][LOCAL_3] == COMIDA_1)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_1], x, y + 7, NULL);
				else if (m.armazem[ARMAZEM_3][LOCAL_3] == COMIDA_2)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_2], x, y, NULL);
				else if (m.armazem[ARMAZEM_3][LOCAL_3] == COMIDA_3)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_3], x, y - 6, NULL);
				else if (m.armazem[ARMAZEM_3][LOCAL_3] == COMIDA_4)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_4], x, y - 12, NULL);

				if (m.armazem[ARMAZEM_3][LOCAL_4] == COMIDA_1)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_1], x, y, NULL);
				else if (m.armazem[ARMAZEM_3][LOCAL_4] == COMIDA_2)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_2], x, y - 7, NULL);
				else if (m.armazem[ARMAZEM_3][LOCAL_4] == COMIDA_3)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_3], x, y - 13, NULL);
				else if (m.armazem[ARMAZEM_3][LOCAL_4] == COMIDA_4)
					al_draw_bitmap(m.imgComidaAmazem[COMIDA_4], x, y - 19, NULL);
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
	//Aumenta o tamanho da visão da formiga, efeito do item TOCHA
	if (it.tocha)
		al_draw_bitmap(m.imgSombra[GRANDE], sombraX, sombraY, NULL);
	//Renderiza o efeito que impede a visão da formiga
	else
		al_draw_bitmap(m.imgSombra[PEQUENA], sombraX, sombraY, NULL);
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
void inicializarAllegro(Controle& c) {
	al_init();
	al_set_new_display_flags(ALLEGRO_OPENGL);
	c.display = al_create_display(LARGURA, ALTURA);
	al_set_window_title(c.display, "The Ant Quest");
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_install_keyboard();
	al_install_audio();
	al_init_acodec_addon();
	al_reserve_samples(6);
	c.fila_eventos = al_create_event_queue();
	al_register_event_source(c.fila_eventos, al_get_keyboard_event_source());
	al_register_event_source(c.fila_eventos, al_get_display_event_source(c.display));
}

// Verifica se o jogador venceu (CARLOS)
bool venceu(int a[QUANTIDADE_ARMAZENS][QUANTIDADE_LOCAIS]) {
	if (a[ARMAZEM_2][LOCAL_1] == COMIDA_4 &&
		a[ARMAZEM_2][LOCAL_2] == COMIDA_3 &&
		a[ARMAZEM_2][LOCAL_3] == COMIDA_2 &&
		a[ARMAZEM_2][LOCAL_4] == COMIDA_1 ||
		a[ARMAZEM_3][LOCAL_1] == COMIDA_4 &&
		a[ARMAZEM_3][LOCAL_2] == COMIDA_3 &&
		a[ARMAZEM_3][LOCAL_3] == COMIDA_2 &&
		a[ARMAZEM_3][LOCAL_4] == COMIDA_1)
		return true;
	else
		return false;
}

// Tela de fim de jogo (CARLOS)
void fimJogo(Controle& c) {
	int x = 180, y = 260;
	std::string tempoString = std::to_string(c.tempoExecucao);
	char const* tempoChar = tempoString.c_str();
	if (c.venceu) {
		tocarSample(c.somVitoria, c.sfx);
		c.venceu = false;
	}
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
				tocarSample(c.somSelecionar, c.sfx);
				c.reiniciar = true;
			}
		}
	}
}

// Sorteia os armazém pelo jogo (CARLOS E MATEUS)
void posicionarArmazens(Mapa& m) {
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

// Inicializa o loop da musica tema (CARLOS)
void inicializarAudio(Controle& c) {
	c.tema = al_create_sample_instance(c.somTema);
	al_set_sample_instance_playmode(c.tema, ALLEGRO_PLAYMODE_LOOP);
	al_attach_sample_instance_to_mixer(c.tema, al_get_default_mixer());
}

// Reinicia as variáveis (CARLOS)
void inicializarJogo(Mapa& m, Controle& c, Formiga& f, Item& i) {
	inicializarVariaveis(m, c, f, i);
	novoMapa(m);
	posicionarFormiga(m, f);
	iniciarArmazem(m.armazem);
	posicionarItens(m, i);
	posicionarArmazens(m);
}

// função para medir tempo e sortear novo mapa (CARLOS E MATEUS)
void medirTempo(bool inicio, Mapa& m, Controle& c, Formiga& f, Item& i) {
	if (inicio)
		c.tempoInicial = clock();
	else {
		c.tempoFinal = clock();
		c.tempoTotal -= (c.tempoFinal - c.tempoInicial) / (double)CLOCKS_PER_SEC;
		c.tempoExecucao += (c.tempoFinal - c.tempoInicial) / (double)CLOCKS_PER_SEC;

		if (c.tempoTotal < 0) {
			tocarSample(c.somTerremoto, c.sfx);
			novoMapa(m);
			posicionarFormiga(m, f);
			posicionarItens(m, i);
			posicionarArmazens(m);
			c.tempoTotal = TEMPO_MAXIMO;
		}
	}
}
