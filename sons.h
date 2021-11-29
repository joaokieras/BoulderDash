// Projeto desenvolvido por João Pedro Kieras Oliveira
// GRR 20190379 Dinf - UFPR
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "game.h"

#ifndef __SONS__
#define __SONS__

struct audio{
  ALLEGRO_SAMPLE* terra;
  ALLEGRO_SAMPLE* diamante;
  ALLEGRO_SAMPLE* fall;
  ALLEGRO_SAMPLE* win;
  ALLEGRO_AUDIO_STREAM* bg_music;
};
typedef struct audio audio;

audio* inicializa_sons();
ALLEGRO_SAMPLE* carrega_audio(char* nome_arquivo);
void play_sound(ALLEGRO_SAMPLE* som);
void destroy_sounds(audio* sons);

#endif