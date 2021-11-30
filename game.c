// Projeto desenvolvido por João Pedro Kieras Oliveira
// GRR 20190379 Dinf - UFPR
#include <stdio.h>
#include <stdlib.h>
#include "allegro5/allegro_ttf.h"
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
//ALLEGRO_BITMAP* background;

player *jogador;
objetos *objetos_mapa;
audio *sons_jogo;

int **mapa, relogio = 150;
long frames = 0;
unsigned char key[ALLEGRO_KEY_MAX];
char *cheat_code = "prog";

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

  //font = al_create_builtin_font();
  inicia_allegro(al_init_font_addon(), "fonte");
  inicia_allegro(al_init_ttf_addon(), "fonte");
  font = al_create_builtin_font();
  font = al_load_ttf_font("resources/fonts/pixeboy.ttf", 25, 0);
  inicia_allegro(font, "font");

  inicia_allegro(al_init_primitives_addon(), "primitives");

  al_register_event_source(queue, al_get_keyboard_event_source());
  al_register_event_source(queue, al_get_display_event_source(disp));
  al_register_event_source(queue, al_get_timer_event_source(timer));

  //background = al_load_bitmap("resources/sprites/background.png");

  state = JOGANDO;
}

void state_serve(){
  bool done = false;
  al_flush_event_queue(queue);
  while(1){
  	al_wait_for_event(queue, &event);
  	if(al_is_event_queue_empty(queue))
  	  draw_instructions();
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
  	  	if(!strcmp(jogador->code, cheat_code))
  	      jogador->invencivel = 1;
  	  	if(!jogador->invencivel)
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
  	  jogador->vidas = 0;
  	}
  	if(jogador->vidas < 1 && frames % TEMPO_RESET == 0){
  	  state = FIMPART;
  	  break;
  	}
  	if(done)
      break;
    if(testa_game_win(mapa, jogador)){
      ganhou = 1;
      jogador->pontuacao = jogador->pontuacao + (relogio * 10);
  	  draw(redraw, frames);
  	  play_sound(sons_jogo->win);
  	  state = FIMPART;
  	  //al_rest(1);
  	  break;
  	}
  	if(redraw && al_is_event_queue_empty(queue))
      draw(redraw, frames);
    frames++;
  }
  //state = FIMPART;
}

void state_end(){
  //al_draw_textf(font, al_map_rgb(255, 255, 255), 300, 0, 0, "FIM DO JOGO");
  //al_flip_display();
  bool done = false;
  al_flush_event_queue(queue);
  while(1){
  	al_wait_for_event(queue, &event);
  	if(al_is_event_queue_empty(queue))
  	  draw_end_game();
  	switch(event.type){
  	  case ALLEGRO_EVENT_KEY_DOWN:
        key[event.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
        break;
      case ALLEGRO_EVENT_KEY_UP:
        key[event.keyboard.keycode] &= KEY_RELEASED;
        break;
  	}
  	/*if(key[ALLEGRO_KEY_ENTER]){
  	  //key[ALLEGRO_KEY_ENTER] = 0;
      state = INICIO;
      done = true;
  	}*/
  	if(key[ALLEGRO_KEY_ESCAPE]){
  	  key[ALLEGRO_KEY_ESCAPE] = 0;
  	  state = FIMJOGO;
  	  done = true;
  	}
  	if(done)
  	  break;
  }

  /*if(ganhou)
  	draw_screen_win();
  else
  	draw_screen_lose();*/
  //al_rest(1);
  //state = FIMJOGO;
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

void draw_instructions(){
  //al_clear_to_color(al_map_rgb(0, 0, 0));
  al_draw_filled_rectangle(3 * SIZE_OBJS, 2 * SIZE_OBJS, WIDTH - 3 * SIZE_OBJS, HEIGHT - 1 * SIZE_OBJS, al_map_rgba_f(0, 0, 0, 0.9));
  al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH/4 + 7 * SIZE_OBJS, 20 + 2 * SIZE_OBJS, 0, "I N S T R U C O E S");
  al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH/4, 80 + 2 * SIZE_OBJS, 0, "O objetivo do jogo eh coletar o maximo de diamantes possivel");
  al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH/4, 100 + 2 * SIZE_OBJS, 0, "e chegar na saida antes que o tempo acabe. Cuidado com as");
  al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH/4, 120 + 2 * SIZE_OBJS, 0, "pedras (e diamantes), elas podem cair em cima voce!");
  al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH/4, 160 + 2 * SIZE_OBJS, 0, "Voce pode se mover pelo mapa utilizando as setas do teclado");
  al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH/4, 180 + 2 * SIZE_OBJS, 0, "e pode desistir da partida pressionando ESC.");
  al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH/4, 220 + 2 * SIZE_OBJS, 0, "Dica! Ao utilizar o cheat code 'prog' seu personagem fica invencivel,");
  al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH/4, 240 + 2 * SIZE_OBJS, 0, "mas cuidado! Voce ainda pode ficar preso entre as pedras.");
  al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH/4, 280 + 2 * SIZE_OBJS, 0, "Jogo desenvolvido por Joao Pedro Kieras Oliveira para a");
  al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH/4, 300 + 2 * SIZE_OBJS, 0, "disciplina de Programacao 2.");
  al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH/4, 340 + 2 * SIZE_OBJS, 0, "Departamento de Informatica - UFPR - Novembro de 2021.");
  al_flip_display();
  //redraw = false;
}

void draw_end_game(){
  al_draw_filled_rectangle(3 * SIZE_OBJS, 2 * SIZE_OBJS, WIDTH - 3 * SIZE_OBJS, HEIGHT - 1 * SIZE_OBJS, al_map_rgba_f(0, 0, 0, 0.9));
  al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH/4 + 7 * SIZE_OBJS, 20 + 2 * SIZE_OBJS, 0, "F I M D E J O G O");
  al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH/4, 80 + 2 * SIZE_OBJS, 0, "Pressione ESC para sair...");
  al_flip_display();
}

/*void draw_inicial_menu(){
  al_draw_bitmap(background, 0, 0, 0);
  al_draw_textf(font, al_map_rgb(255, 255, 255), 0, 0, 0, "OPA");
  al_flip_display();
}*/

void draw_hud(){
  al_clear_to_color(al_map_rgb(0, 0, 0));
  al_draw_textf(font, al_map_rgb(255, 255, 255), 250, 10, 0, "%05d", jogador->pontuacao);
  al_draw_bitmap(objetos_mapa->diamante[0], 0, 8, 0);
  al_draw_textf(font, al_map_rgb(255, 255, 255), 20, 10, 0, "%d/%d", jogador->diamantes, MIN_DIAMANTES);
  al_draw_textf(font, al_map_rgb(255, 255, 255), 200, 10, 0, "%d", relogio);
  al_draw_textf(font, al_map_rgb(255, 255, 255), 100, 10, 0, "Vidas: %d", jogador->vidas);
  al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH/2 - 10, 10, 0, "Help: H/F1");
  if(jogador->invencivel)
    al_draw_textf(font, al_map_rgb(255, 255, 255), 800, 10, 0, "invencivel: ON");
  else
  	al_draw_textf(font, al_map_rgb(255, 255, 255), 800, 10, 0, "invencivel: OFF");
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
  else if(key[ALLEGRO_KEY_P]){
  	jogador->code[0] = 'p';
  }
  else if(key[ALLEGRO_KEY_R]){
  	jogador->code[1] = 'r';
  }
  else if(key[ALLEGRO_KEY_O]){
  	jogador->code[2] = 'o';
  }
  else if(key[ALLEGRO_KEY_G]){
  	jogador->code[3] = 'g';
  }
  
  if(key[ALLEGRO_KEY_ESCAPE])
    *done = true;

  for(int i = 0; i < ALLEGRO_KEY_MAX; i++)
    key[i] &= KEY_SEEN;

  redraw = true;
}




