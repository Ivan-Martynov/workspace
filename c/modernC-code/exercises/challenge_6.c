#include <stdio.h>

void print_vec(const size_t N, const double vec[static const N]) {
	for (size_t i = 0; i < N; ++i) {
		printf("%f ", vec[i]);
	}
	printf("\n");
}

void print_mat(const size_t N, const size_t M, const double mat[static const N][M]) {
	for (size_t row = 0; row < N; ++row) {
		print_vec(M, mat[row]);
	}
}

double dot_product(const size_t N, const double vec_1[static const N], const double vec_2[static const N]) {
	double result = 0;

	for (size_t i = 0; i < N; ++i) {
		result += vec_1[i] * vec_2[i];
	}

	return result;
}

/*
double vec_product2(
	const size_t N,
	const double vec_1[static const N],
	const double vec_2[static const N]
) [static 1]  {
	double target[N];
	for (size_t i = 0; i < N; ++i) {
		target[i] = vec_1[i] * vec_2[i];
	}
}
*/

void vec_product(
	const size_t N,
	const double vec_1[static const N],
	const double vec_2[static const N],
	double target[static N]
	) {
	for (size_t i = 0; i < N; ++i) {
		target[i] = vec_1[i] * vec_2[i];
	}
}

void matrix_vector_mult(
	const size_t N, const size_t M,
	const double matrix[static const N][M],
	const double vec[static const M],
	double target[static const N]
) {
	for (size_t row = 0; row < N; ++row) {
		target[row] = dot_product(N, matrix[row], vec);
	}
}

int main() {
	const size_t N = 3;
	const size_t M = 2;
	const double vec_1[M] = {2, 3};
	const double vec_2[M] = {4, 5};
	double target[N] = {};
	const double mat[N][M] = {{1, 4}, {5, 8}, {2, 3}};
	matrix_vector_mult(N, M, mat, vec_1, target);
	print_mat(N, M, mat);
	print_vec(N, target);
	// Warning: different sizes: trailing values of the target vector are unchanged
	vec_product(M, vec_1, vec_2, target);
	print_vec(N, target);
	printf("%f\n", dot_product(N, vec_1, target));
	return 0;
}