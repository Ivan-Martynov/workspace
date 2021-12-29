#include <stdio.h>

int main()
{
    int i, i2;

    scanf("%d", &i);

    freopen("inttext.txt", "w", stdin);

    scanf("%d", &i2);

    printf("Testing freopen()\n");

    freopen("output.txt", "w", stdout);

    //freopen(NULL, "wb", stdout);

#if 0
    FILE *fp;
    if ((fp = fopen("datafile.data", "r")) == NULL) {
        fprintf(stderr, "Could not open file\n");
        return -1;
    }
#endif

    return 0;
}
