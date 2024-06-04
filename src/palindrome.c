#include <stdio.h>
#include <stdbool.h>

int main();
bool is_palindrome(int n);

int main() {
	int n;
	scanf("%d", &n);

	if (is_palindrome(n)) {
		printf("Y\n");
	} else {
		printf("N\n");
	}
	return 0;
}

bool is_palindrome(int n) {
    int ori = n;
    int rev = 0;
    while (n > 0) {
        rev = rev * 10 + n % 10;
        n /= 10;
    }
    return rev == ori;
}

