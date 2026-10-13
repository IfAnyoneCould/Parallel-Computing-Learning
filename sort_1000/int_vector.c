#include "int_vector.h"
#include <stddef.h>

int_vector *create_int_vector(size_t n) {
  int_vector *p = malloc(sizeof(*p));
  if (p) {
    p->data = malloc(n * sizeof(int));
    p->size = n;
  }
  return p;
}

void delete_int_vector(int_vector *v) {
  if (v) {
    free(v->data);
    free(v);
  }
}

size_t resize_int_vector(int_vector *v, size_t n) {
  if (v) {
    int *p = realloc(v->data, n * sizeof(int));
    if (p) {
      v->data = p;
      v->size = n;
    }
    return v->size;
  }
  return 0;
}

int get_int_vector(int_vector *v, size_t n) {
  if (v && n < v->size) {
    return v->data[n];
  }
  return -1;
}

void set_int_vector(int_vector *v, size_t n, int x) {
  if (v) {
    if (n >= v->size) {
      resize_int_vector(v, n + 1);
    }
    v->data[n] = x;
  }
}

void push_int_vector(int_vector *v, int x) { set_int_vector(v, v->size, x); }

int_vector *get_reversed_int_vector(int_vector *v) {
  int_vector *output = create_int_vector(0);
  for (size_t i = v->size - 1; i >= 0; i--) {
    push_int_vector(output, get_int_vector(v, i));
  }
  return output;
}

void reverse_int_vector(int_vector *v) {
  int_vector *output = create_int_vector(0);
  for (size_t i = v->size - 1; i >= 0; i--) {
    push_int_vector(output, get_int_vector(v, i));
  }
  v = output;
}
