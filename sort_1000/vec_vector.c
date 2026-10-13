#include "vec_vector.h"
#include <stddef.h>
#include <stdlib.h>

vec_vector *create_vec_vector(size_t n) {
  vec_vector *v = malloc(sizeof(*v));
  if (v) {
    v->data = NULL;
    v->size = 0;
    resize_vec_vector(v, n);
    return v;
  }
  return NULL;
}

void delete_vec_vector(vec_vector *v) {
  if (!v)
    return;
  for (size_t i = 0; i < v->size; i++) {
    delete_int_vector(v->data[i]);
  }
  free(v->data);
  free(v);
}

size_t resize_vec_vector(vec_vector *v, size_t n) {
  for (size_t i = n; i < v->size; i++) {
    delete_int_vector(v->data[i]);
  }
  int_vector **p = realloc(v->data, n * sizeof(*p));
  if (!p && n > 0)
    return v->size;
  v->data = p;

  for (size_t i = v->size; i < n; i++) {
    v->data[i] = create_int_vector(0);
  }

  v->size = n;
  return v->size;
}

int_vector *get_vec_vector(vec_vector *v, size_t n) {
  if (v && n < v->size) {
    return v->data[n];
  }
  return NULL;
}

void set_vec_vector(vec_vector *v, size_t n, int_vector *x) {
  if (v) {
    if (n >= v->size) {
      resize_vec_vector(v, n + 1);
    }
    delete_int_vector(v->data[n]);
    v->data[n] = x;
  }
}

int get_vec_vector_elem(vec_vector *v, size_t row, size_t col) {
  int_vector *p = get_vec_vector(v, row);
  return get_int_vector(p, col);
}

void set_vec_vector_elem(vec_vector *v, size_t row, size_t col, int x) {
  int_vector *p = get_vec_vector(v, row);
  set_int_vector(p, col, x);
}

int_vector *get_vec_vector_sizes(vec_vector *v) {
  int_vector *p = create_int_vector(v->size);
  for (size_t i = 0; i < v->size; i++) {
    set_int_vector(p, i, v->data[i]->size);
  }
  return p;
}
