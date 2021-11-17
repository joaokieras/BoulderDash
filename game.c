// Projeto desenvolvido por João Pedro Kieras Oliveira
// GRR 20190379 Dinf - UFPR
#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "mapa.h"
#include "player.h"
#include "sprites.h"

ALLEGRO_TIMER* timer;
ALLEGRO_EVENT event;
ALLEGRO_EVENT_QUEUE* queue;
ALLEGRO_DISPLAY* disp;
ALLEGRO_FONT* font;
ALLEGRO_BITMAP* sheet;

player *jogador;
objetos *objetos_mapa;

int **mapa, **mapa_anterior;
long frames = 0;

void inicia_allegro(bool teste, char *descricao){
  if(teste) 
  	return;
  fprintf(stderr, "Não foi possivel inicializar %s\n", descricao);
  exit(1);
}

void state_init(){
  inicia_allegro(al_init(), "allegro");
  inicia_allegro(al_install_keyboard(), "keyboard");

  timer = al_create_timer(1.0 / FRAMERATE);
  inicia_allegro(timer, "timer");

  queue = al_create_event_queue();
  inicia_allegro(queue, "queue");

  inicia_allegro(al_init_image_addon(), "image addon");
  sheet = al_load_bitmap(PATH_SPRITESHEET);
  inicia_allegro(sheet, "spritesheet"); 
  jogador = inicia_jogador(sheet);
  objetos_mapa = inicia_objetos(sheet);
  mapa = inicia_mapa(PATH_MAP_1);
  mapa_anterior = inicia_mapa_anterior();

  //al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
  //al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
  //al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

  disp = al_create_display(WIDTH, HEIGHT);
  inicia_allegro(disp, "display");

  font = al_create_builtin_font();
  inicia_allegro(font, "font");

  inicia_allegro(al_init_primitives_addon(), "primitives");

  al_register_event_source(queue, al_get_keyboard_event_source());
  al_register_event_source(queue, al_get_display_event_source(disp));
  al_register_event_source(queue, al_get_timer_event_source(timer));

  state = SERVINDO;
}

void state_serve(){
  state = JOGANDO;
}

void state_play(){
  bool done = false;
  bool redraw = true;
  long frames = 0;
  unsigned char key[ALLEGRO_KEY_MAX];

  memset(key, 0, sizeof(key));
  al_start_timer(timer);

  while(1){
  	al_wait_for_event(queue, &event);
  	switch(event.type){
  	  case ALLEGRO_EVENT_TIMER:
  	  	verifica_entradas(key, &done, redraw, jogador);
  	  	//atualiza_mapa_anterior(mapa, mapa_anterior);
  	  	/*for(int i = 0;i < 22;i++){
  	  	  for(int j = 0;j < 40;j++){
  	  	  	if(mapa[i][j] == PEDRA && frames % 10 == 0)
  	  	  	  testa_desmoronamento(mapa, objetos_mapa, i, j, frames);
  	  	  }
  	  	}*/
  	  	break;
  	  case ALLEGRO_EVENT_KEY_DOWN:
        key[event.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
        break;
      case ALLEGRO_EVENT_KEY_UP:
        key[event.keyboard.keycode] &= KEY_RELEASED;
        break;
      case ALLEGRO_EVENT_DISPLAY_CLOSE:
        done = true;
        break;
  	}
  	if(done)
      break;
    if(redraw && al_is_event_queue_empty(queue))
      draw(jogador, redraw, frames);
    frames++;
  }
  state = FIMPART;
}

void state_end(){
  al_draw_textf(font, al_map_rgb(255, 255, 255), 300, 0, 0, "FIM DO JOGO");
  al_flip_display();
  usleep(1000000);
  state = FIMJOGO;
}

void state_close(){
  al_destroy_bitmap(sheet);
  al_destroy_font(font);
  al_destroy_display(disp);
  al_destroy_timer(timer);
  al_destroy_event_queue(queue);
  exit(1);
}

void draw(player *jogador, bool redraw, long frames){
  al_clear_to_color(al_map_rgb(0, 0, 0));
  al_draw_textf(font, al_map_rgb(255, 255, 255), 0, 0, 0, "X: %d Y: %d", jogador->pos_x/SIZE_OBJS, (jogador->pos_y - MARGIN_TOP)/SIZE_OBJS);
  al_draw_textf(font, al_map_rgb(255, 255, 255), 500, 0, 0, "PONTOS: %d", jogador->pontuacao);
  al_draw_textf(font, al_map_rgb(255, 255, 255), 600, 0, 0, "DIMAS: %d", jogador->diamantes);
  al_draw_textf(font, al_map_rgb(255, 255, 255), 150, 0, 0, "Frames: %ld", frames);
  draw_map(mapa, mapa_anterior, objetos_mapa, frames);
  draw_player(jogador, mapa, frames);
  al_flip_display();
  redraw = false;
}

void verifica_entradas(unsigned char *key, bool *done, bool redraw, player *jogador){
  if(key[ALLEGRO_KEY_UP]){
  	if((jogador->pos_y - SIZE_OBJS - MARGIN_TOP) > 0){
      jogador->flag_up = 1;
      jogador->tired++;
    }
  }
  else if(key[ALLEGRO_KEY_DOWN]){
  	if(jogador->pos_y < HEIGHT - SIZE_OBJS - MARGIN_TOP){
      jogador->flag_down = 1;
      jogador->tired++;
    }
  }
  else if(key[ALLEGRO_KEY_LEFT]){
  	if(jogador->pos_x > SIZE_OBJS){
      jogador->flag_left = 1;
      jogador->tired++;
    }
  }
  else if(key[ALLEGRO_KEY_RIGHT]){
  	if(jogador->pos_x < WIDTH - 2 * SIZE_OBJS){
      jogador->flag_right = 1;
      jogador->tired++;
    }
  }
  
  if(key[ALLEGRO_KEY_ESCAPE])
    *done = true;

  for(int i = 0; i < ALLEGRO_KEY_MAX; i++)
    key[i] &= KEY_SEEN;

  redraw = true;
}




