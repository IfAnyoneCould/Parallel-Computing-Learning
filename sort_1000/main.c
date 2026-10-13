#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "vec_vector.h"

uint32_t xor_state;
uint32_t get_rand(int lo, int hi) {
  uint32_t x = xor_state;
  x ^= x << 13;
  x ^= x >> 17;
  x ^= x << 5;
  xor_state = x;
  return lo + x % (hi - lo + 1);
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

void merge_piles(int *src, int *dst, int lo, int mid, int hi) {
  size_t a_pos = lo, b_pos = mid, out_pos = lo;

  while (a_pos < mid && b_pos < hi) {
    int a_elem = src[a_pos];
    int b_elem = src[b_pos];

    if (a_elem > b_elem) {
      dst[out_pos] = b_elem;
      b_pos += 1;
    } else {
      dst[out_pos] = a_elem;
      a_pos += 1;
    }
    out_pos += 1;
  }

  if (out_pos != hi) {
    int pos = a_pos < mid ? a_pos : b_pos;
    for (size_t i = 0; i < hi - out_pos; i++) {
      dst[out_pos + i] = src[i + pos];
    }
  }
}

vec_vector *deal_piles(int *nums, int size) {
  vec_vector *piles = create_vec_vector(0);

  if (!piles)
    return NULL;

  for (size_t i = 0; i < size; i++) {
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

void patience_sort(int *nums, int size) {
  vec_vector *piles = deal_piles(nums, size);
  reverse_all_vec_vector(piles);

  int_vector *flattened = flatten_vec_vector(piles);

  int *src = malloc(size * sizeof(int));
  int *dst = malloc(size * sizeof(int));
  size_t pile_size = piles->size;
  int starts[pile_size + 1];
  size_t runs = pile_size;

  memcpy(src, get_array_int_vector(flattened), size * sizeof(int));
  delete_int_vector(flattened);

  starts[0] = 0;

  for (size_t i = 1; i < pile_size; i++) {
    int_vector *vec = get_vec_vector(piles, i - 1);
    starts[i] = starts[i - 1] + vec->size;
  }

  starts[pile_size] = size;
  delete_vec_vector(piles);

  while (runs > 1) {
    int new_starts[pile_size + 1];
    for (size_t i = 0; i < runs; i += 2) {
      if (i + 1 < runs) {
        merge_piles(src, dst, starts[i], starts[i + 1], starts[i + 2]);
      } else {
        for (size_t j = starts[i]; j < starts[i + 1]; j++) {
          dst[j] = src[j];
        }
      }
      new_starts[i / 2] = starts[i];
    }

    int *temp = src;
    src = dst;
    dst = temp;

    runs = (runs + 1) / 2;
    new_starts[runs] = size;
    memcpy(starts, new_starts, sizeof(starts));
  }
  free(dst);
  memcpy(nums, src, size * sizeof(int));
  free(src);
}

bool verify_sorted(int *nums, size_t size) {
  for (size_t i = 1; i < size; i++) {
    if (nums[i] < nums[i - 1]) {
      return false;
    }
  }
  return true;
}

static int cmp_int(const void *a, const void *b) {
  int x = *(const int *)a, y = *(const int *)b;
  return (x > y) - (x < y);
}

static void qsort_int(int *a, int n) { qsort(a, n, sizeof(int), cmp_int); }

static double time_sort(void (*sort)(int *, int), int *input, int *work, int n,
                        int trials) {
  double best = 1e300;
  for (int t = 0; t < trials; t++) {
    memcpy(work, input, n * sizeof(int));
    clock_t t0 = clock();
    sort(work, n);
    double ms = (double)(clock() - t0) * 1000.0 / CLOCKS_PER_SEC;
    if (ms < best)
      best = ms;
  }
  return best;
}

int main() {
  xor_state = (uint32_t)time(NULL);

  int sizes[] = {1000, 10000, 100000, 1000000};
  int trials = 5;

  printf("%10s %12s %12s %8s\n", "n", "patience ms", "qsort ms", "correct");
  for (size_t s = 0; s < sizeof(sizes) / sizeof(sizes[0]); s++) {
    int n = sizes[s];
    int *input = gen_rand_array(n, 0, 1000);
    int *ref = malloc(n * sizeof(int));
    int *work = malloc(n * sizeof(int));

    memcpy(ref, input, n * sizeof(int));
    qsort_int(ref, n);

    double p = time_sort(patience_sort, input, work, n, trials);
    bool ok = verify_sorted(work, n) && memcmp(work, ref, n * sizeof(int)) == 0;
    double q = time_sort(qsort_int, input, work, n, trials);

    printf("%10d %12.2f %12.2f %8s\n", n, p, q, ok ? "yes" : "NO");

    free(input);
    free(ref);
    free(work);
  }
  return 0;
}
