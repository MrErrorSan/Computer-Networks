#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main()
{
	FILE* f;
	char chr;
	f= fopen("input.txt","r");

	do {
        chr = fgetc(f);
        printf("%c", chr);
    	} 
    while (chr != EOF);
    printf("\n");
    fclose(f);
    return 0;
return 0;
}
