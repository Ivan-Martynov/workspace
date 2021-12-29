/*
The prime factors of 13195 are 5, 7, 13 and 29.
What is the largest prime factor of the number 600851475143 ?
*/

#include <stdio.h>
#include <stdbool.h>

int show_factor(const int k, const long int i) {
  if (k > 0) {
    if (k > 1) {
      printf(" * %ld^%d", i, k);
    } else {
      printf(" * %ld", i);
    }
  }

  return 0;
}

void prime_factorization(long int n) {

  printf("%ld = 1", n);

  int k = 0;
  while (n % 2 == 0) {
    n /= 2;
    ++k;
  }

  k = show_factor(k, 2);

  while (n % 3 == 0)
  {
    n /= 3;
    ++k;
  }

  k = show_factor(k, 3);

  for (long int i = 5; i <= n; i += 6) {
    while (n % i == 0) {
      n /= i;
      ++k;
    }

    k = show_factor(k, i);

    while (n % (i + 2) == 0)
    {
      n /= (i + 2);
      ++k;
    }

    k = show_factor(k, i + 2);
  }

  printf("\n");
}

long int largest_prime(long int n) {
  if (n < 2) {
    return -1;
  }

  long int r = n;
  for (long int i = 2; i <= n; i++) {
    while (n % i == 0) {
      n /= i;
      r = i;
    }
  }

  return r;
}

bool isPrime(const long int n) {
  if (n < 0) {
    return isPrime(-n);
  }

  if (n == 2 || n == 3) {
    return true;
  }

  if (n < 2 || n % 2 == 0 || n % 3 == 0) {
    return false;
  }

  for (long int i = 5; i * i <= n; i += 6) {
    if (n % i == 0 || n % (i + 2) == 0) {
      return false;
    }
  }
}

int main() {
  const long int n = 600851475143;
  printf("Is number %ld prime? = %s\n", n, isPrime(n) ? "true" : "false");
  printf("Largest prime for %ld = %ld\n", n, largest_prime(n));

  prime_factorization(n);

  return 0;
}