// Projeto desenvolvido por João Pedro Kieras Oliveira
// GRR 20190379 Dinf - UFPR 
#ifndef __SPRITES__
#define __SPRITES__

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include "game.h"
#include "mapa.h"
#include "player.h"

void inicia_sprites_objetos(ALLEGRO_BITMAP* sheet, objetos* obj);
void inicia_sprites_jogador(ALLEGRO_BITMAP* sheet, player* jogador);
void destroi_sprites_objetos(objetos* obj);
void destroi_sprites_player(player* jogador);
#endif