#include <lib.h>
#include <string.h>

int flag = 0;

char buf[30];

const char input[30] = "this is what father env write";

static void os_assert(int cond, const char *err) {
	if (!cond) {
		user_halt("%s\n", err);
	}
}

void check() {
	while(1) {}
}

int main() {
	u_int child_stack = 0x7f3fd800;
	for (int i = 1 ; i <= 100; i++) {
	if (syscall_clone((void *)check, (void *)child_stack) != -14) {
		debugf("%d, %d\n",i, vpd[PDX(KSEG0)]);
	}
	}
	strcpy(buf, input);
	flag = 1;
	return 0;
}
