#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main()
{
	FILE* f;
	FILE* f1;
	char buff[255];
	f= fopen("input.txt","r");
	f1=fopen("d_output.txt","w");
	while(!feof(f))
	{
		
		fscanf(f,"%s",buff);
		int i=0;
		int check=0;
		while(i<strlen(buff))
		{
			if(check==0)
			if(buff[i]=='a'|| buff[i]=='e'|| buff[i]=='i'|| buff[i]=='o'|| buff[i]=='u')
			{
				check=1;
				int start =0;
				int end= strlen(buff)-1; 
				char temp;
   			 	while (start < end)
    			{
        			temp = buff[start];  
        			buff[start] = buff[end];
        			buff[end] = temp;
        			start++;
        			end--;
				}
			}
			i++;
		}
	fputs(buff,f1);
	fputc(' ',f1);
	}
    //printf("\n");
    fclose(f);
    return 0;
return 0;
}
