#include <stdio.h>

static int NUM = 0;

void test(int*, int);

int main(void) {
	printf("HELLO!!: %d\n", NUM);

	{
		printf("\n我进来了哦\n\n");
	}

	printf("TYPE: %d", sizeof(typeof(1.0)));

	printf("%X", 0/0);

	int a = 50;
	test(&a, 333);
	test(&a, 21);
	test(&a, 34);

	return 0;
}

void test(int* num, int rng) {
	NUM += *num = rng;
	printf("\nNUM: %d, *num: %d, rng: %d\n", NUM, *num, rng);
}

