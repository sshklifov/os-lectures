#include <stdio.h>

int main(int argc, char** argv) {
	int sum = 0;
	for (int i = 1; i < argc; ++i) {
		int number;
		sscanf(argv[i], "%d", &number);
		sum += number;
	}

	printf("final sum is = %d\n", sum);
	return 0;
}
