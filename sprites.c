// Projeto desenvolvido por João Pedro Kieras Oliveira
// GRR 20190379 Dinf - UFPR
#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "mapa.h"
#include "player.h"
#include "sprites.h"

void inicia_sprites_objetos(ALLEGRO_BITMAP* sheet, objetos* obj){
  obj->metal = al_create_sub_bitmap(sheet, 0, 48, 15, 16);
  obj->saida = al_create_sub_bitmap(sheet, 16, 48, 15, 16);
  obj->muro  = al_create_sub_bitmap(sheet, 32, 48, 15, 16);
  obj->terra = al_create_sub_bitmap(sheet, 48, 48, 15, 16);
  obj->pedra = al_create_sub_bitmap(sheet, 80, 48, 15, 16);
  obj->vazio = al_create_sub_bitmap(sheet, 96, 48, 15, 16);

  obj->explosao[3] = al_create_sub_bitmap(sheet, 96, 48, 15, 16);
  obj->explosao[2] = al_create_sub_bitmap(sheet, 112, 64, 15, 16);
  obj->explosao[1] = al_create_sub_bitmap(sheet, 128, 64, 15, 16);
  obj->explosao[0] = al_create_sub_bitmap(sheet, 128, 80, 15, 16);

  obj->diamante[0] = al_create_sub_bitmap(sheet, 0, 64, 15, 16);
  obj->diamante[2] = al_create_sub_bitmap(sheet, 0, 80, 15, 16);
  obj->diamante[4] = al_create_sub_bitmap(sheet, 0, 96, 15, 16);
  obj->diamante[6] = al_create_sub_bitmap(sheet, 0, 112, 15, 16);
  obj->diamante[1] = al_create_sub_bitmap(sheet, 16, 64, 15, 16);
  obj->diamante[3] = al_create_sub_bitmap(sheet, 16, 80, 15, 16);
  obj->diamante[5] = al_create_sub_bitmap(sheet, 16, 96, 15, 16);
  obj->diamante[7] = al_create_sub_bitmap(sheet, 16, 112, 15, 16);
}

void inicia_sprites_jogador(ALLEGRO_BITMAP* sheet, player* jogador){
  jogador->player_parado[0] = al_create_sub_bitmap(sheet, 0, 0, 15, 16);
  for(int i = 1;i <= 6;i++)
    jogador->player_parado[i] = al_create_sub_bitmap(sheet, (16 * i), 0, 15, 16);	

  jogador->player_esquerda[0] = al_create_sub_bitmap(sheet, 0, 16, 15, 16);
  for(int i = 1;i <= 6;i++)
    jogador->player_esquerda[i] = al_create_sub_bitmap(sheet, (16 * i), 16, 15, 16);	

  jogador->player_direita[0] = al_create_sub_bitmap(sheet, 0, 32, 15, 16);
  for(int i = 1;i <= 6;i++)
    jogador->player_direita[i] = al_create_sub_bitmap(sheet, (16 * i), 32, 15, 16);
}