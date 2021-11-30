// Projeto desenvolvido por João Pedro Kieras Oliveira
// GRR 20190379 Dinf - UFPR

#include <stdio.h>
#include <stdlib.h>
#include "game.h"

int main(){
  state = INICIO;

  while(1){
  	switch(state){
  	  case INICIO: state_init(); break;
  	  case SERVINDO: state_serve(); break;
  	  case JOGANDO: state_play(); break;
  	  case FIMPART: state_end(); break;
  	  case FIMJOGO: state_close(); break;
  	}
  }
  return 0;
}

