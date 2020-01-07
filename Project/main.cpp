/*
* DESCRIÇÃO: Jogo criado para a disciplina de Algoritmos e Programação.
* AUTORES:
* Carlos Eduardo de Borba Machado
* Caio Gonzaga Bernils
* Mateus Luis Ropke Lauer
*/

// BIBLIOTECAS
//
#include "game.h"
//

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
	inicializarAudio(c);

	/*------------------------------ LOOP PRINCIPAL ---------------------------*/

	while (!c.sair) {
		al_clear_to_color(al_map_rgb(0, 0, 0));
		//Inicia o menu do jogo
		if (c.menu)
			menu(c);
		// Inicia o jogo chamando todas as funções
		else if (c.jogar) {
			medirTempo(true, m, c, f, i);
			tempoItem(true, i);
			lerComandos(m, c, f, i);
			desenharFrame(m, c, f, i);
			tempoItem(false, i);
			medirTempo(false, m, c, f, i);
		}
		// Ativa o menu de pausa
		else if (c.pausa)
			pausa(m, c, f, i);
		// Finaliza o jogo após vencer
		if (venceu(m.armazem) && f.vazio)
			fimJogo(c);
		if (c.reiniciar)
			inicializarJogo(m, c, f, i);

		al_set_sample_instance_playing(c.tema, c.musica);
		if (c.jogar)
			al_set_sample_instance_gain(c.tema, 0.25);
		else
			al_set_sample_instance_gain(c.tema, 1.0);

		al_flip_display();
	}

	/*-------------------------------------------------------------------------*/

	finalizar(m, c, f, i);

	return 0;
}