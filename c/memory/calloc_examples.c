#include <stdio.h>
#include <stdlib.h>

typedef struct
{
  int num;
  char* info;
} record;

int main()
{
  record* recs;
  int num_recs = 2;
  char str[] = "Information";

  recs = calloc(num_recs, sizeof(record));

  if (recs != NULL)
  {
    for (int k = 0; k < num_recs; k++)
    {
      (recs + k)->num = k;
      (recs + k)->info = str;
      puts((recs + k)->info);
    }
  }

  free(recs);

  return 0;
}