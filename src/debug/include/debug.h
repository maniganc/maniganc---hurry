/**
 *   \file debug.h
 *   \brief helper functions/macros for debugging code
 */

#ifndef DEBUG_H
#define DEBUG_H

#include <stdarg.h>
#include <stdio.h>

#ifndef DEBUG
#define DEBUG 0
#endif

#ifdef DEBUG
#undef DEBUG
#define DEBUG 1
#endif

#define STRINGIFY(s) #s

#define debug(...)                                                      \
  do {                                                                  \
    if(DEBUG) {                                                         \
      printf("[debug %s:%d %s()]", __FILE__, __LINE__, __func__);       \
      printf(__VA_ARGS__);                                              \
    }                                                                   \
  }                                                                     \
  while (0);

#define error(...)                                                      \
  do {                                                                  \
    fprintf(stderr, "[%s(%d) %s()] |error| ", __FILE__, __LINE__, __func__); \
    fprintf(stderr, __VA_ARGS__);                                       \
  }                                                                     \
  while (0);

#define assert(valid_expr, ...)                                         \
  do {                                                                  \
    if(DEBUG && !(valid_expr)) {                                        \
      printf("[%s(%d) %s()] |assert : %s| ",                            \
             __FILE__, __LINE__, __func__, STRINGIFY(valid_expr));      \
      printf(__VA_ARGS__);                                              \
    }                                                                   \
  }                                                                     \
  while (0);

#endif /* DEBUG_H */
