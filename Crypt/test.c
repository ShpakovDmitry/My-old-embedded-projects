#include <stdio.h>

int main()
{
	char *str1 = __DATE__;
	char *str2 = __TIME__;
	char *str3 = __FILE__;
	
	printf("%s %s %s\n", str1, str2, str3);
	
	return 0;
}
