#include <stdio.h>

int main(int argc, char *argv[])
{
	char key[] = "012345678987654321";
	int i;
	int j = 0;
	
	printf("%s -> ", argv[1]);
	for(i = 0; argv[1][i]!='\0'; i++)
	{
		printf("%c", argv[1][i]^key[j]);
		j++;
		if(key[j] == '\0')
			j = 0;
		
	}
	putchar('\n');
	
	
	//printf("%c XOR %c = %c\n", a, b, a^b);
	//printf("%d%d%d%d%d%d%d%d\n", b&128, b&64, b&32, b&16, b&8, b&4, b&2, b&1);
	
	return 0;
}
