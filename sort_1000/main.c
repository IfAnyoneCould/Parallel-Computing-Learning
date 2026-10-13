#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "vec_vector.h"

uint32_t xor_state;
uint32_t get_rand(int lo, int hi) {
  uint32_t x = xor_state;
  x ^= x << 13;
  x ^= x >> 17;
  x ^= x << 5;
  x = x % (hi - lo + 1);
  return xor_state = x;
}

int *gen_rand_array(int size, int lo, int hi) {
  int *arr = malloc(size * sizeof(int));
  if (arr == NULL)
    return NULL;
  for (int i = 0; i < size; i++) {
    arr[i] = get_rand(lo, hi);
  }
  return arr;
}

int_vector *merge_piles(int_vector *a, int_vector *b) {
  int_vector *output = create_int_vector(a->size + b->size);

  size_t a_pos = 0, b_pos = 0, out_pos = 0;

  while (a_pos < a->size && b_pos < b->size) {
    int a_elem = get_int_vector(a, a_pos);
    int b_elem = get_int_vector(b, b_pos);

    if (a_elem > b_elem) {
      set_int_vector(output, out_pos, b_elem);
      b_pos += 1;
    } else {
      set_int_vector(output, out_pos, a_elem);
      a_pos += 1;
    }
    out_pos += 1;
  }

  if (out_pos != output->size) {
    bool which = a_pos < a->size;
    int_vector *remainder = which ? a : b;
    int pos = which ? a_pos : b_pos;
    for (size_t i = 0; i < output->size - out_pos; i++) {
      set_int_vector(output, out_pos + i, get_int_vector(remainder, i + pos));
    }
  }

  return output;
}
vec_vector *patience_sort(int *nums, int size) {
  vec_vector *piles = create_vec_vector(0);

  if (!piles)
    return NULL;

  set_vec_vector_elem(piles, 0, 0, nums[0]);

  for (size_t i = 1; i < size; i++) {
    int lo = 0;
    int hi = piles->size;

    while (lo < hi) {
      int mid = lo + (hi - lo) / 2;
      int_vector *mid_pile = get_vec_vector(piles, mid);
      int top = get_int_vector(mid_pile, mid_pile->size - 1);
      if (top >= nums[i]) {
        hi = mid;
      } else {
        lo = mid + 1;
      }
    }

    if (lo == piles->size) {
      resize_vec_vector(piles, piles->size + 1);
      set_int_vector(get_vec_vector(piles, piles->size - 1), 0, nums[i]);

    } else {
      int_vector *lo_pile = get_vec_vector(piles, lo);
      set_int_vector(lo_pile, lo_pile->size, nums[i]);
    }
  }

  return piles;
}

int main() {
  xor_state = (uint32_t)time(NULL);

  int arr_size = 10;

  int *nums = gen_rand_array(arr_size, 0, 1000);

  for (int i = 0; i < arr_size; i++) {
    printf("%d ", nums[i]);
  }
  printf("\n");

  vec_vector *test = patience_sort(nums, arr_size);

  for (size_t i = 0; i < test->size; i++) {
    int_vector *pile = get_vec_vector(test, i);
    for (size_t h = 0; h < pile->size; h++) {
      printf("%d, ", get_int_vector(pile, h));
    }
    printf("\n\n");
  }

  int_vector *a = get_vec_vector(test, 0);
  int_vector *a_rev = get_reversed_int_vector(a);

  int_vector *b = get_vec_vector(test, 1);
  int_vector *b_rev = get_reversed_int_vector(b);

  int_vector *m = merge_piles(a_rev, b_rev);

  for (size_t i = 0; i < m->size; i++) {
    printf("%d ", get_int_vector(m, i));
  }
  printf("\n");

  return 0;
}
