void scanf_1_check() {
	printk("Running scanf_1_check\n");
	char str[100];
	char str2[100];
	char ch;

	scanf("%s%c%s", str, &ch,str2);
	printk("Finished 1st scanf\n");
	printk("%s %c %s\n", str, ch, str2);
}

void mips_init(u_int argc, char **argv, char **penv, u_int ram_low_size) {
	scanf_1_check();
	halt();
}
