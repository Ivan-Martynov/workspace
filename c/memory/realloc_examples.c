#include <stdio.h>
#include <stdlib.h>

int main()
{
  int* ptr = malloc(3 * sizeof(*ptr));

  if (ptr != NULL)
  {
    *(ptr + 2) = 23;
  }

  for (int i = 0; i < 3; i++)
  {
    printf("%d ", *(ptr + i));
  }
  puts("\n");

  ptr = realloc(ptr, 5 * sizeof(*ptr));

  if (ptr != NULL)
  {
    *(ptr + 3) = 47;
  }

  for (int i = 0; i < 5; i++)
  {
    printf("%d ", *(ptr + i));
  }
  puts("\n");

  return 0;
}