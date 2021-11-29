// Projeto desenvolvido por João Pedro Kieras Oliveira
// GRR 20190379 Dinf - UFPR
#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "mapa.h"
#include "player.h"
#include "sprites.h"
#include "sons.h"

ALLEGRO_TIMER* timer;
ALLEGRO_EVENT event;
ALLEGRO_EVENT_QUEUE* queue;
ALLEGRO_DISPLAY* disp;
ALLEGRO_FONT* font;
ALLEGRO_BITMAP* sheet;

player *jogador;
objetos *objetos_mapa;
audio *sons_jogo;

int **mapa, relogio = 150;
long frames = 0;
unsigned char key[ALLEGRO_KEY_MAX];

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

  inicia_allegro(al_install_audio(), "audio");
  inicia_allegro(al_init_acodec_addon(), "audio codecs");
  inicia_allegro(al_reserve_samples(16), "reserve samples");

  jogador = inicia_jogador(sheet);
  objetos_mapa = inicia_objetos(sheet);
  mapa = inicia_mapa(PATH_MAP_1, objetos_mapa);
  sons_jogo = inicializa_sons();
  al_set_audio_stream_playmode(sons_jogo->bg_music, ALLEGRO_PLAYMODE_LOOP);
  //al_attach_audio_stream_to_mixer(sons_jogo->bg_music, al_get_default_mixer());

  al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
  al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
  al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

  disp = al_create_display(WIDTH, HEIGHT);
  inicia_allegro(disp, "display");

  font = al_create_builtin_font();
  inicia_allegro(font, "font");

  inicia_allegro(al_init_primitives_addon(), "primitives");

  al_register_event_source(queue, al_get_keyboard_event_source());
  al_register_event_source(queue, al_get_display_event_source(disp));
  al_register_event_source(queue, al_get_timer_event_source(timer));

  state = JOGANDO;
}

void state_serve(){
  bool done = false;
  int draw = 1;
  al_flush_event_queue(queue);
  while(1){
  	al_wait_for_event(queue, &event);
  	al_draw_textf(font, al_map_rgb(255, 255, 255), 200, 20, 0, "PAUSE");
  	if(al_is_event_queue_empty(queue))
  	  draw_instructions(draw, frames);
  	al_flip_display();
  	switch(event.type){
  	  case ALLEGRO_EVENT_KEY_DOWN:
        key[event.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
        break;
      case ALLEGRO_EVENT_KEY_UP:
        key[event.keyboard.keycode] &= KEY_RELEASED;
        break;
  	}
  	if(key[ALLEGRO_KEY_H] || key[ALLEGRO_KEY_F1]){
  	  key[ALLEGRO_KEY_H] = 0;
      key[ALLEGRO_KEY_F1] = 0;
      state = JOGANDO;
      done = true;
  	}
  	else if(key[ALLEGRO_KEY_ESCAPE]){
  	  key[ALLEGRO_KEY_ESCAPE] = 0;
  	  state = FIMPART;
  	  done = true;
  	}
  	if(done)
  	  break;
  }
  //state = JOGANDO;
}

void state_play(){
  bool done = false;
  bool redraw = true;
  long frames = 0;
  int morreu = 0, ganhou = 0;

  memset(key, 0, sizeof(key));
  al_start_timer(timer);

  while(1){
  	al_wait_for_event(queue, &event);
  	switch(event.type){
  	  case ALLEGRO_EVENT_TIMER:
  	  	verifica_entradas(key, &done, redraw, jogador);
  	  	verifica_min_diamantes(mapa, jogador);
  	  	testa_desmoronamento_pedra(mapa, sons_jogo, objetos_mapa, frames);
  	  	testa_desmoronamento_diamante(mapa, sons_jogo, objetos_mapa, frames);
  	  	morreu = testa_game_over(mapa, sons_jogo, objetos_mapa, frames, relogio);
  	  	if(morreu)
  	  	  reseta_player(jogador);
  	  	if(frames % 60 == 0 && jogador->vivo)
  	  	  relogio--;
  	  	if(!jogador->vivo && frames % TEMPO_RESET == 0){
  	  	  mapa = inicia_mapa(PATH_MAP_1, objetos_mapa);
  	  	  relogio = 150;
  	  	}
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
  	if(key[ALLEGRO_KEY_H] || key[ALLEGRO_KEY_F1]){
  	  key[ALLEGRO_KEY_H] = 0;
      key[ALLEGRO_KEY_F1] = 0;
      state = SERVINDO;
      done = true;
  	}
  	if(key[ALLEGRO_KEY_ESCAPE]){
  	  key[ALLEGRO_KEY_ESCAPE] = 0;
  	  state = FIMPART;
  	  done  = true;
  	}
  	if(jogador->vidas < 1 && frames % TEMPO_RESET == 0){
  	  break;
  	}
  	if(done)
      break;
    if(testa_game_win(mapa, jogador)){
      jogador->pontuacao = jogador->pontuacao + (relogio * 10);
  	  draw(redraw, frames);
  	  play_sound(sons_jogo->win);
  	  al_rest(2);
  	  break;
  	}
  	if(redraw && al_is_event_queue_empty(queue))
      draw(redraw, frames);
    frames++;
  }
  //state = FIMPART;
}

void state_end(){
  al_draw_textf(font, al_map_rgb(255, 255, 255), 300, 0, 0, "FIM DO JOGO");
  al_flip_display();
  al_rest(1);
  state = FIMJOGO;
}

void state_close(){
  destroi_sprites_objetos(objetos_mapa);
  destroi_sprites_player(jogador);
  destroy_sounds(sons_jogo);
  destroi_mapa(mapa);
  destroi_objetos(objetos_mapa);
  destroi_player(jogador);
  al_destroy_bitmap(sheet);
  al_destroy_font(font);
  al_destroy_display(disp);
  al_destroy_timer(timer);
  al_destroy_event_queue(queue);
  exit(1);
}

void draw(bool redraw, long frames){
  draw_hud();
  draw_map(mapa, sons_jogo, objetos_mapa, frames);
  draw_player(jogador, sons_jogo, mapa, objetos_mapa, frames);
  al_flip_display();
  redraw = false;
}

void draw_instructions(bool redraw, long frames){
  //al_clear_to_color(al_map_rgb(0, 0, 0));
  al_draw_filled_rectangle(2 * SIZE_OBJS, 2 * SIZE_OBJS, WIDTH - 4 * SIZE_OBJS, HEIGHT - 1 * SIZE_OBJS, al_map_rgba_f(0, 0, 0, 0.9));
  al_draw_textf(font, al_map_rgb(255, 255, 255), 425 + 2 * SIZE_OBJS, 20 + 2 * SIZE_OBJS, 0, "I N S T R U Ç Õ E S");
  al_draw_textf(font, al_map_rgb(255, 255, 255), 425 + 2 * SIZE_OBJS, 80 + 2 * SIZE_OBJS, 0, "I N S T R U Ç Õ E S");
  al_draw_textf(font, al_map_rgb(255, 255, 255), 425 + 2 * SIZE_OBJS, 100 + 2 * SIZE_OBJS, 0, "I N S T R U Ç Õ E S");
  al_draw_textf(font, al_map_rgb(255, 255, 255), 425 + 2 * SIZE_OBJS, 120 + 2 * SIZE_OBJS, 0, "I N S T R U Ç Õ E S");
  al_flip_display();
  //redraw = false;
}

void draw_hud(){
  al_clear_to_color(al_map_rgb(0, 0, 0));
  al_draw_textf(font, al_map_rgb(255, 255, 255), 800, 10, 0, "PONTOS: %05d", jogador->pontuacao);
  al_draw_textf(font, al_map_rgb(255, 255, 255), 0, 10, 0, "DIMAS: %d/%d", jogador->diamantes, MIN_DIAMANTES);
  al_draw_textf(font, al_map_rgb(255, 255, 255), 700, 10, 0, "%d", relogio);
  al_draw_textf(font, al_map_rgb(255, 255, 255), 100, 10, 0, "Vidas: %d", jogador->vidas);
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




