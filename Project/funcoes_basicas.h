#pragma once

// BIBLIOTECAS
//
#include <allegro5/allegro_audio.h>
//

// Responsável por tocar o sample (CARLOS)
void tocarSample(ALLEGRO_SAMPLE* sample, bool sfx) {
	if (sfx)
		al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
}