#ifndef INT_VECTOR_H
#define INT_VECTOR_H

#include <stddef.h>
#include <stdlib.h>

typedef struct int_vector {
  size_t size;
  size_t capacity;
  int *data;
} int_vector;

int_vector *create_int_vector(size_t n);
void delete_int_vector(int_vector *v);
size_t resize_int_vector(int_vector *v, size_t n);
int get_int_vector(int_vector *v, size_t n);
void set_int_vector(int_vector *v, size_t n, int x);
int_vector *get_reversed_int_vector(int_vector *v);
int *get_array_int_vector(int_vector *v);

#endif
