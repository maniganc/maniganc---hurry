
#include <stdio.h>
#include <stdlib.h>
#include "debug.h"

#define TYPE char
#include "vector_template.h"

typedef int (*ss_func_t)(void);

typedef struct {
  ss_func_t get_next_func;

  vector_char char_buffer;

  int save;

  int current_char_idx;

  int checkpoint_number;
}ss_t;

void ss_init(ss_t* s, ss_func_t func) {
  s->get_next_func = func;
  vector_char_init(&s->char_buffer);
  s->save = 0;
  s->current_char_idx = 0;

  int ch = s->get_next_func();
  vector_char_push(&s->char_buffer, (char)ch);

  s->checkpoint_number = 0;
}

int ss_get_current(ss_t* s) {
  return vector_char_get_idx(&s->char_buffer, s->current_char_idx);
}

void ss_next(ss_t* s) {
  if (s->save) {
    /* save mode */
    int last_char_idx = vector_char_get_size(&s->char_buffer) - 1;
    if (s->current_char_idx < last_char_idx) {
      /* still some next char left */
      debug("still some next char left");
      s->current_char_idx++;
      return;
    }

    else {
      /* no next char left */
      debug("no next char left, asking it");
      int ch = s->get_next_func();

      if (s->checkpoint_number != 0) {
        debug("still some checkpoints, need to buffer it");
        vector_char_push(&s->char_buffer, (char)ch);
        s->current_char_idx++;
      }

      else {
        debug("no more checkpoints, stop buffering");
	vector_char_erase(&s->char_buffer);
        s->current_char_idx = 0;
        vector_char_set_idx(&s->char_buffer, 0, (char)ch);
	s->save = 0;
      }

    }
  }

  else {
    /* no save */
    debug("no save, asking");
    int ch = s->get_next_func();
    s->current_char_idx = 0;
    vector_char_set_idx(&s->char_buffer, 0, (char)ch);
  }
}

void ss_previous(ss_t* s) {
  if (s->checkpoint_number > 0) {
    if (s->current_char_idx == 0) {
      error("can't go back more");
    }

    else {
      s->current_char_idx--;
    }
  }

  else {
    /* nope */
    error("can't go back, no save");
  }
}

int ss_mark_current(ss_t* s) {
  s->save = 1;
  s->checkpoint_number++;
  return s->current_char_idx;
}

void ss_load_mark(ss_t* s, int mark) {
  s->current_char_idx = mark;
}

void ss_drop_mark(ss_t* s) {
  if (s->checkpoint_number > 0) {
    s->checkpoint_number--;
  }
  else {
    error("Can't drop more mark : none remaining");
  }

}

int askchar(void) {

  printf("char? ");
  int ch;
  do {
    ch = getchar();
  }
  while (ch == '\n');
  printf("\n");
  return ch;
}

int main(int argc, char *argv[]) {
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  ss_t s;
  ss_init(&s, askchar);

  while(1) {
    int c = getchar();
    if (c == EOF) {
      return 0;
    }

    switch (c) {
    case 'C': {
      printf("<Current>");
      int ch = ss_get_current(&s);
      printf("-> %c\n", ch);
      break;
    }
    case 'N': {
      printf("<Next>");
      ss_next(&s);
      break;
    }
    case 'P': {
      printf("<Previous>");
      ss_previous(&s);
      break;
    }
    case 'M': {
      int m = ss_mark_current(&s);
      printf("<Mark @%d>", m);
      break;
    }
    case 'L': {
      printf("<Load mark> num?");
      int m; scanf("%d", &m);
      ss_load_mark(&s, m);
      break;
    }
    case 'D': {
      printf("<Drop mark>");
      ss_drop_mark(&s);
      break;
    }

    default:
      break;
    }

  }
  return 0;
}
