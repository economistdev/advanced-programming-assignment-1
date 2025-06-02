
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

bool is_digit_in_number(int x, int num) {

	int temp_num = 0;
	while (num != 0) {
		temp_num = num % 10;
		if (x == temp_num) return true;
		num /= 10;
	}
	return false;
	
}


void recursive_solve(int *solution_addr, int x, int n, int curr_a) {

	int max_a = n/curr_a;

	if (curr_a > max_a) { // don't need to calculate futher than half the size of n
		return;
	}

	int b = n - curr_a;
	if (!(is_digit_in_number(x, curr_a) || is_digit_in_number(x, b))) {
		solution_addr[0] = curr_a;
		solution_addr[1] = b;
		return;
	}

	return recursive_solve(solution_addr, x, n, curr_a + 1);
	
}

void removeDigit(int x, int n, int *a, int *b) {

	if (a == NULL || b == NULL) {
		return;
	}

	if (x < 0 || x > 9) return;

	if (n < 10 || n > 100000000) return;

	int solution_addr[2] = {0};
	recursive_solve(solution_addr, x, n, 1);
	*a = solution_addr[0];
	*b = solution_addr[1];

}

void test() {

	int x = 1;
	int n = 1000000;
	int a, b;
	a = b = 0;

	removeDigit(x, n, &a, &b);
	printf("a=%d, and b=%d \n", a, b);

}

int main() {
	
	//printf("%d\n", );
	test();
	
	return 0;
}	

