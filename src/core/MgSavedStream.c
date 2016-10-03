
#include "MgSavedStream.h"

#include "debug.h"

#define BUFFER_DEFAULT_INIT_SIZE 32


static void reset_static_vars(MgSavedStream* s) {
  s->buffer_char_flag = 0;
  s->current_char_idx = 0;
  s->mark_counter = 0;

  s->init = 0;

  s->line_number = 1;
};
void MgSavedStream_init(MgSavedStream* s,
                        MgSavedStream_getchar_func getchar,
                        void* getchar_payload) {
  reset_static_vars(s);  
  s->getchar = getchar;
  s->getchar_payload = getchar_payload;
  vector_char_init(&s->buffer);
}

void MgSavedStream_deinit(MgSavedStream* s) {
  vector_char_deinit(&s->buffer);
}

void MgSavedStream_reset(MgSavedStream* s) {
  reset_static_vars(s);
  vector_char_erase(&s->buffer);
}

static void line_metric_update_when_next(MgSavedStream* s) {
  int c = MgSavedStream_get_current(s);
  if (c == '\n') {
    s->line_number++;
  }
}

static void line_metric_update_when_previous(MgSavedStream* s) {
  int c = MgSavedStream_get_current(s);
  if (c == '\n' && s->line_number > 0) {
    s->line_number--;
  }
}

size_t MgSavedStream_get_line_number(MgSavedStream* s) {
  return s->line_number;
}

int MgSavedStream_get_current(MgSavedStream* s) {
  if (!s->init) {
    s->init = 1;
    int c = s->getchar(s->getchar_payload);
    vector_char_push(&s->buffer, (char)c);
  }

  return vector_char_get_idx(&s->buffer, s->current_char_idx);
}

int MgSavedStream_get_next(MgSavedStream* s) {
  if (!s->init) {
    MgSavedStream_get_current(s);
  }

  if (s->buffer_char_flag) {
    /* buffer next chars */
    size_t last_char_idx = vector_char_get_size(&s->buffer) - 1;
    if (s->current_char_idx < last_char_idx) {
      /* still some char stored to be used as next char */
      s->current_char_idx++;
    }

    else {
      /* no next char left in buffer */
      /* ask one char */
      int c = s->getchar(s->getchar_payload);

      if (s->mark_counter > 0) {
        /* still some checkpoints : if there is a need to go back to them,
         * continue to buffer chars
         */
        vector_char_push(&s->buffer, (char)c);
        s->current_char_idx++;
      }

      else {
        /* no more checkpoints, no need to continue buffering chars */
        vector_char_erase(&s->buffer);
        vector_char_push(&s->buffer, (char)c);
        s->current_char_idx = 0;
        s->buffer_char_flag = 0;
      }

    }
  }

  else {
    /* no save */
    int c = s->getchar(s->getchar_payload);
    vector_char_set_idx(&s->buffer, 0, (char)c);
  }

  line_metric_update_when_next(s);
  return MgSavedStream_get_current(s);
}

int MgSavedStream_get_previous(MgSavedStream* s) {
  if (s->mark_counter > 0) {
    if (s->current_char_idx == 0) {
      /* can't go back more */
      assert(0, "cannot go back further in the saved stream");
      return -1;
    }

    else {
      s->current_char_idx--;
      line_metric_update_when_previous(s);
      return MgSavedStream_get_current(s);
    }
  }

  else {
    /* cannot */
    assert(0, "stream not saved, going back is not possible");
    return -1;
  }
}

MgSavedStream_mark MgSavedStream_mark_current(MgSavedStream* s) {
  /* save next chars */
  s->buffer_char_flag = 1;

  s->mark_counter++;
  
  MgSavedStream_mark m;
  m.char_idx = s->current_char_idx;
  m.line_number = s->line_number;
  return m;
}

void MgSavedStream_load_mark(MgSavedStream* s, MgSavedStream_mark mark) {
  s->current_char_idx = mark.char_idx;
  s->line_number = mark.line_number;
}

void MgSavedStream_drop_mark(MgSavedStream* s) {
  if (s->mark_counter > 0) {
    s->mark_counter--;
  }

  else {
    assert(0, "cannot drop mark : none remaining");
  }
}
