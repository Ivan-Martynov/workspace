#include <stdio.h>
#include <stdbool.h>

#include "sorting_algos.h"

/*
**  Utils
*/
void swap_voids(unsigned char* const arg1, unsigned char* const arg2, size_t size) {
	for (size_t i = 0; i < size; ++i) {
		const unsigned char tmp = arg1[i];
		arg1[i] = arg2[i];
		arg2[i] = tmp;
	}
}

void print_array_general(const void* ptr, const size_t count, const size_t size,
	void (*print_element)(const void* const)) {
	for (size_t i = 0; i < count; ++i) {
		print_element(ptr);
		ptr += size;
	}
	printf("\n");
}

void print_array_d(const double arr[static 1], const size_t size) {
	for (size_t i = 0; i < size; ++i) {
		printf("%f ", arr[i]);
	}
	printf("\n");
}

bool is_sorted_d(const double arr[static 1], const size_t count) {
	for (size_t i = 1; i < count; i++) {
		if (arr[i - 1] > arr[i]) {
			return false;
		}
	}

	return true;
}

bool is_sorted_general(void* ptr, const size_t count, const size_t size,
	int(*compare)(const void*, const void*)) {
	for (size_t i = 1; i < count; i++) {
		void* next = ptr + size;
		
		if (compare(ptr, next) > 0) {
			return false;
		}
		
		ptr = next;
	}

	return true;
}


/*
** Bubble sorting
*/

void bubble_sort_d(double arr[static 1], size_t count) {
	while (true) {
		bool no_change = true;
		
		for (size_t i = 1; i < count; i++) {
			if (arr[i - 1] > arr[i]) {
				double t = arr[i - 1];
				arr[i - 1] = arr[i];
				arr[i] = t;
				
				no_change = false;
			}
		}
		
		if (no_change) {
			break;
		}

		--count; // the last element is in its place
	}
}

void bubble_sort_general(void* ptr, size_t count, const size_t size, int (*compare)(const void*, const void*)) {
	while (true) {
		bool no_change = true;
		
		void* prev = ptr;
		for (size_t i = 1; i < count; i++) {
			void* next = prev + size;
			
			if (compare(prev, next) > 0) {
				swap_voids(prev, next, size);
				no_change = false;
			}
			
			prev = next;
		}

		if (no_change) {
			break;
		}
		
		--count; // the last element is in its place, no need to check
	}
}

/*
** Merge sorting
*/

void merge_d(double arr[static 1], const size_t left, const size_t middle, const size_t right)
{
	size_t first_n = middle - left + 1;
	size_t second_n = right - middle;

	double first_subarray[first_n];
	double second_subarray[second_n];

	for (size_t i = 0; i < first_n; ++i) {
		first_subarray[i] = arr[left + i];
	}

	for (size_t j = 0; j < second_n; ++j) {
		second_subarray[j] = arr[middle + 1 + j];
	}

	size_t i = 0;
	size_t j = 0;
	size_t k = left;

	while (i < first_n && j < second_n) {
		if (first_subarray[i] < second_subarray[j]) {
			arr[k++] = first_subarray[i++];
		} else {
			arr[k++] = second_subarray[j++];
		}
	}

	while (i < first_n) {
		arr[k++] = first_subarray[i++];
	}

	while (j < second_n) {
		arr[k++] = second_subarray[j++];
	}
}

void swap_doubles(double* a, double* b) {
	double temp = *a;
	*a = *b;
	*b = temp;
}

void merge_sort_d(double arr[static 1], const size_t left, const size_t right)
{
	if (left < right) {
		size_t middle = left + ((right - left) >> 1);

		merge_sort_d(arr, left, middle);
		merge_sort_d(arr, middle + 1, right);

		merge_d(arr, left, middle, right);
	}
}

size_t partition_d(double array[static 1], const size_t left, const size_t right) {
	double pivot = array[right];

	size_t k = left;

	for (size_t i = left; i < right; ++i) {
		if (array[i] < pivot) {
			if (k < i) {
				swap_doubles(&array[i], &array[k]);
			}
			++k;
		}
	}

	swap_doubles(&array[k], &array[right]);

	return k;
}

void quick_sort_d(double arr[static 1], const size_t left, const size_t right) {
	if (left < right) {
		size_t index = partition_d(arr, left, right);

		quick_sort_d(arr, left, index - 1);
		quick_sort_d(arr, index + 1, right);
	}
}


