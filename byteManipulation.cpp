#include <stdio.h>

int main(void)
{
	long lang = 0b00000001000000000000000000000000;

	unsigned char a = lang >> 24;
	long b = a << 24;
	unsigned char c = lang >> 8;
	unsigned char d = lang;

	printf("lang: %d a: %d, b: %d, c: %d, d: %d\n", lang, a, b, c, d);	

	return(1);
}
