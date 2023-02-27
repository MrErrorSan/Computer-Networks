#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main()
{
	FILE* f;
	FILE* f1;
	char buff[255];
	int c=0;
	f= fopen("input.txt","r");
	f1=fopen("c_output.txt","w");
	while(!feof(f))
	{
		
		fscanf(f,"%s",buff);
		int i=0; c=0;
		while(i<strlen(buff))
		{
			if(buff[i]>=65 && buff[i]<=90)
			{
				c=1;
				break;
			}else if(buff[i]>=97 && buff[i]<=122)
				{
					c=1;
					break;
				}
			i++;
		}
		if(c==0)
		{
			fputs(buff,f1);
			fputc(' ',f1);
		}
	}
    //printf("\n");
    fclose(f);
    return 0;
return 0;
}
