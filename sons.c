#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "game.h"
#include "sons.h"

ALLEGRO_SAMPLE* carrega_audio(char* nome_arquivo){
  ALLEGRO_SAMPLE* audio = al_load_sample(nome_arquivo);
  inicia_allegro(audio, "arquivo de audio");
  return audio;
}

audio* inicializa_sons(){
  audio* sons;
  sons = malloc(sizeof(audio));

  sons->terra = carrega_audio("resources/audios/terra.ogg");
  sons->diamante = carrega_audio("resources/audios/diamante.ogg");
  sons->fall = carrega_audio("resources/audios/fall.ogg");
  sons->win = carrega_audio("resources/audios/win.wav");
  sons->lose = carrega_audio("resources/audios/lose.wav");
  sons->cheat = carrega_audio("resources/audios/cheat.wav");
  sons->bg_music = al_load_audio_stream("resources/audios/bg_music.ogg", 2, 2048);
  inicia_allegro(sons->bg_music, "musica");
  return sons;
}

void play_sound(ALLEGRO_SAMPLE* som){
  al_play_sample(som, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
}

void destroy_sounds(audio* sons){
  al_destroy_sample(sons->terra);
  al_destroy_sample(sons->diamante);
  al_destroy_sample(sons->fall);
  al_destroy_sample(sons->win);
  al_destroy_sample(sons->lose);
  al_destroy_sample(sons->cheat);
  al_destroy_audio_stream(sons->bg_music);
}