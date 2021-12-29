/* select() demo */
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#define STDIN 0 // file descriptor for standart input

int main(void)
{
	struct timeval tv;
	fd_set readfs;

	tv.tv_sec = 2;
	tv.tv_usec = 5e5;

	FD_ZERO(&readfs); // clear all entries of readfs
	FD_SET(STDIN, &readfs); // set standart input to readfs?

	/* don't care about writefds and exceptfds */
	select(STDIN + 1, &readfs, NULL, NULL, &tv);

	if (FD_ISSET(STDIN, &readfs))
		printf("A key was pressed!\n");
	else
		printf("Time out.\n");

	return 0;
}
