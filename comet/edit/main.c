#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	FILE *f = fopen("output.dat","r");
	
	if(f==NULL)
	{
		printf("Can't open file\n");
		return -1;
	}
	
	FILE *g = fopen("data.dat","w");
	if(g==NULL)
	{
		printf("Can't create or write to file\n");
		fclose(f);
		return -1;
	}
	
	
	int i,j,k;
	char st[9][20];
	
	for(i=0;i<2001;i++)
	{
		fscanf(f,"%s %s %s %s %s %s %s %s %s", st[0],st[1],st[2],st[3],st[4],st[5],st[6],st[7],st[8]);
		
		for(j=0;j<9;j++)
		{
			for(k=0;st[j][k]!='\0';k++)
			{
				if(st[j][k]=='.')
				{
					st[j][k]=',';
				}
			}
		}
		
		fprintf(g,"%s %s %s %s %s %s %s %s %s\n", st[0],st[1],st[2],st[3],st[4],st[5],st[6],st[7],st[8]);
	}
	
	
	/*char str[10]="5.36485";
	
	for(i=0; i<20 || (str[i]!='\0');i++)
	{
		if(str[i]=='.')
		{
			str[i]=',';
		}
	}	
	printf("%s\n", str);*/

	
	fclose(f);
	fclose(g);

	return 0;
}
