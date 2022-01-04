#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

void singleton_partition(size_t parent[static 1], size_t count) {
	for (size_t i = 0; i < count; ++i) {
		parent[i] = SIZE_MAX;
	}
}

int FindRoot(size_t parent[static 1], size_t count, size_t index) {
	if (parent[index] == SIZE_MAX) {
		return index;
	}

	return parent[index] == index ? index : FindRoot(parent, count, parent[index]);
}

void make_set(size_t count, size_t array[static count]) {
	for (size_t i = 0; i < count; ++i) {
		array[i] = i;
	}
}

size_t Find(size_t count, size_t array[static count], size_t i) {
	if (i >= count) {
		return SIZE_MAX;
	}

	return array[i] == i ? i : Find(count, array, array[i]);
}

void FindReplace(size_t count, size_t array[static count], size_t i, size_t value) {
	if (i >= count) {
		return;
	}

	if (i == array[i]) {
		array[i] = value;
	} else {
		size_t index = array[i];

		array[i] = value;

		FindReplace(count, array, index, value);
	}
}

size_t FindCompress(const size_t count, size_t array[static count], size_t i) {
	if (i >= count) {
		return SIZE_MAX;
	}

	if (i != array[i]) {
		array[i] = FindCompress(count, array, array[i]);
	}

	return array[i];
}

void Union(const size_t count, size_t array[static count], size_t i, size_t j) {
	if (i >= count || j >= count) {
		return;
	}

	size_t k = FindCompress(count, array, j);

	FindReplace(count, array, i, k);
}

void print_array(const size_t count, size_t array[static count]) {
	for (size_t i = 0; i < count; ++i) {
		printf("%zu ", array[i]);
	}
	printf("\n");
}

int main() {

	const size_t n = 10;
	size_t parent[n];

	singleton_partition(parent, n);

	size_t index = 3;
	size_t r = FindRoot(parent, n, index);
	printf("Root of the tree at index %zu: %zu\n", index, r);


	print_array(n, parent);

	FindReplace(n, parent, 5, 3);
	FindReplace(n, parent, 3, 6);
	print_array(n, parent);

	const size_t i = Find(n, parent, 5);
	printf("%zu\n", i);

	Union(n, parent, 0, 1);
	print_array(n, parent);

	return 0;
}