
void fun() {
}

int main() {
	fun (10, "GFG", 3.03);
	static int i = 5;

	if (--i) {
		printf("%d ", i);
		main(10);
	}

	return 0;
}
