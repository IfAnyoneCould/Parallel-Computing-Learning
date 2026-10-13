#ifndef VEC_VECTOR_H
#define VEC_VECTOR_H

#include <stddef.h>
#include <stdlib.h>

#include "int_vector.h"

typedef struct vec_vector {
  size_t size;
  int_vector **data;
} vec_vector;

vec_vector *create_vec_vector(size_t n);
void delete_vec_vector(vec_vector *v);
size_t resize_vec_vector(vec_vector *v, size_t n);
int_vector *get_vec_vector(vec_vector *v, size_t n);
void set_vec_vector(vec_vector *v, size_t n, int_vector *x);
int get_vec_vector_elem(vec_vector *v, size_t row, size_t col);
void set_vec_vector_elem(vec_vector *v, size_t row, size_t col, int x);
int_vector *get_vec_vector_sizes(vec_vector *v);
void push_int_vector(int_vector *v, int x);
void reverse_int_vector(int_vector *v);

#endif
