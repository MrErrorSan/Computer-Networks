#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main()
{
	FILE* f;
	FILE* f1;
	char chr;
	f= fopen("input.txt","r");
	f1=fopen("b_output.txt","w");
	do {
        chr = fgetc(f);
        if(chr>48 && chr<58)
        	{
        		printf("%c", chr);
        		fputc(chr,f1);
        	}
    	} 
    while (chr != EOF);
    printf("\n");
    fclose(f);
    return 0;
return 0;
}
