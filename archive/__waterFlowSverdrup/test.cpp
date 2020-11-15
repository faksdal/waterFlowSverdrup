#include <stdio.h>
int main(void)
{
	int	buffer[64];
	int	x = 3, y = 2, width = 128;

	buffer[ x + (y/8) * width] |= (1 << (y & 7));

	printf("x:				%d\n", x);
	printf("y:				%d\n", y);
	printf("(y & 7):			%d\n", (y & 7));
	printf("1 << (y & 7):			%d\n", (1 << (y & 7)));
	printf("buffer[ x + (y/8) * width]:	%d\n", buffer[ x + (y/8) * width]);
	
	return 1;
}

