#pragma once

// BIBLIOTECAS
//
#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
//
#include "defs.h"
#include "funcoes_basicas.h"
//

// Faz o menu principal (CARLOS, MATEUS E CAIO)
void menuPrincipal(Controle& c, const char* st1, const char* st2, const char* st3, const char* st4, bool& recordes, bool& musica, bool principal) {
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
				tocarSample(c.somSelecionar, c.sfx);
				c.opcao--;
				if (c.opcao < JOGAR)
					c.opcao = JOGAR;
				break;
				// Caso a tecla S ou Seta para baixo forem pressionadas, seleciona a opção de baixo
			case ALLEGRO_KEY_S:
			case ALLEGRO_KEY_DOWN:
				tocarSample(c.somSelecionar, c.sfx);
				c.opcao++;
				if (c.opcao > SAIR)
					c.opcao = SAIR;
				break;
			case ALLEGRO_KEY_ENTER:
			case ALLEGRO_KEY_SPACE:
				tocarSample(c.somEnter, c.sfx);
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
	// Posiciona as opções do Menu (CARLOS, MATEUS E CAIO)
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
		al_draw_text(c.fonte[T50], al_map_rgb(0, 0, 0), x + 425, y + 660, NULL, st4);
	}
	else {
		al_draw_bitmap(c.imgSelecao[NAO_SELECIONADO], x + 305, y + 650, NULL);
		al_draw_text(c.fonte[T50], al_map_rgb(255, 255, 255), x + 425, y + 660, NULL, st4);
	}
}

// Faz o menu recordes (CARLOS, MATEUS E CAIO)
void menuRecordes(Controle c, bool& recordes) {
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
				tocarSample(c.somEnter, c.sfx);
				recordes = false;
				c.opcao = JOGAR;
				break;
			}
		}
	}
	// Posiciona as opções do Menu
	al_draw_bitmap(c.imgRecordes, x + LARGURA / 2 - 145, y + 330, NULL);
	al_draw_multiline_text(c.fonte[T20], al_map_rgb(0, 0, 0), x + 365, y + 420, 250, 35.0, NULL, "this functionality has not been implemented yet");
	al_draw_bitmap(c.imgSelecao[SELECIONADO], x + 305, y + 650, NULL);
	al_draw_text(c.fonte[T50], al_map_rgb(0, 0, 0), x + 400, y + 660, NULL, "back");
}

// Faz o menu musica (CARLOS, MATEUS E CAIO)
void menuMusica(Controle& c, bool& musica) {
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
				tocarSample(c.somSelecionar, c.sfx);
				c.opcao--;
				if (c.opcao < MUSICA)
					c.opcao = MUSICA;
				break;
			case ALLEGRO_KEY_S:
			case ALLEGRO_KEY_DOWN:
				tocarSample(c.somSelecionar, c.sfx);
				c.opcao++;
				if (c.opcao > VOLTAR)
					c.opcao = VOLTAR;
				break;
			case ALLEGRO_KEY_ENTER:
			case ALLEGRO_KEY_SPACE:
				tocarSample(c.somEnter, c.sfx);
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
		al_draw_bitmap(c.imgSelecao[NAO_SELECIONADO], x + 305, y + 550, NULL);
		al_draw_text(c.fonte[T50], al_map_rgb(255, 255, 255), x + 400, y + 560, NULL, "back");
	}
}

// Apresenta a tela de menu ao jogador (CARLOS E CAIO)
void menu(Controle& c) {
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
void pausa(Mapa& m, Controle& c, Formiga& f, Item& i) {
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
void formigaAtual(Mapa& m, Formiga f) {
	if (f.vazio)
		m.mapa[f.y][f.x] = FORMIGA_VAZIO;
	else
		m.mapa[f.y][f.x] = FORMIGA_CHEIO;
}
