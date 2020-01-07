#pragma once

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
	NORMAL,
	DESMORONANDO
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
	bool menu, jogar, pausa, sair, reiniciar, musica, sfx, venceu;
	double tempoTotal, tempoExecucao;
	int opcao;
	clock_t tempoInicial, tempoFinal;
	ALLEGRO_EVENT_QUEUE* fila_eventos;
	ALLEGRO_FONT* fonte[TOTAL_FONTES];
	ALLEGRO_DISPLAY* display;
	ALLEGRO_BITMAP* imgPlaca, * imgMenu, * imgBota, * imgTocha, * imgPa, * imgRelogio, * imgSelecao[2], * imgRecordes;
	ALLEGRO_SAMPLE_INSTANCE* tema;
	ALLEGRO_SAMPLE* somTema, * somEnter, * somSelecionar, * somCavar, * somTocha, * somKit, * somPa, * somFolha, * somCouro, * somCaminhada, * somVitoria, * somTerremoto;
};

typedef struct Formiga {
	int comidaAtual, x, y, DIRECAO, tempoVelocidade, velocidade;
	bool vazio;
	ALLEGRO_BITMAP* imgFormiga[4];
};

typedef struct Mapa {
	int armazem[QUANTIDADE_ARMAZENS][QUANTIDADE_LOCAIS], ** mapa, x, y;
	ALLEGRO_BITMAP* imgParede, * imgCaminho, * imgBackground, * imgComida[4], * imgSombra[2], * imgComidaAmazem[4], * imgArmazem[2];
};

typedef struct Item {
	int quantidadePa;
	bool tocha, bota;
	double tempoTocha, tempoFinalTocha, tempoInicialTocha, tempoBota, tempoFinalBota, tempoInicialBota;
	ALLEGRO_BITMAP* imgPa, * imgBota, * imgTocha, * imgKit;
};

/*-----------------------------------------------------------------------------*/