#include <stdio.h>

/*
If we list all the natural numbers below 10 that are multiples of 3 or 5,
we get 3, 5, 6 and 9. The sum of these multiples is 23.
Find the sum of all multiples of 3 or 5 below 1000.
*/

int sum_helper(const int a, const int b, const int n) {
  return (a + b) * n;
}

int multiples(const int i, const int j, const int n) {
  const int ij = i * j;
  const int m1 = (n - 1) / i;
  const int m2 = (n - 1) / j;
  const int m3 = (n - 1) / ij;
  const int res =\
    (sum_helper(i, m1 * i, m1) +\
    sum_helper(j, m2 * j, m2) -\
    sum_helper(ij, (m3 * ij), m3)) / 2;
  return res;
}

int main() {
  const int i = 3;
  const int j = 5;
  const int n = 1000;
  const int r = multiples(3, 5, 1000);

  printf("sum of multiples of %d and %d below %d = %d\n", i, j, n, r);

  return 0;
}