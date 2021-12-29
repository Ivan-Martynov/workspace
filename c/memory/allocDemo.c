#include <stdlib.h>
#include <stdio.h>

#define INITIAL_SIZE 10
#define BUCKET_SIZE  5

static int data_count; // how many ints
static int data_size;  // how many ints we can store
static int *data;      // the block of data

int main(void)
{
    void add_data(int new_data); // function prototype
    int i;

    // init the data area
    data_count = 0;
    data_size = INITIAL_SIZE;
    data = malloc(data_size * sizeof(int));

    // add a bunch of data
    for (i = 0; i < 23; i++) {
        add_data(i);
    }
    printf("%d\n", data_size);

    for (i = 0; i <= data_size; i++) {
        printf("%d\n", *(data + i));
        printf("%d\n", data[i]);
    }
    return 0;
}

void add_data(int new_data)
{
    // if data_count == data_size the area is full and needs to be
    // realloc()'d before we can add more

    if (data_count == data_size) {
        // full up, add a bucket
        data_size += BUCKET_SIZE;
        data = realloc(data, data_size * sizeof(int));
    }

    // store the data
    *(data + data_count) = new_data;

    // the above line could have used array notation:
    // data[data_count] = new_data;

    data_count++;
}
