#include <stdio.h>

void Mem_rep(char *start, int n)
{
    int i;
    for (i = 0; i < n; i++)
         printf(" %.2x", start[i]);
    printf("\n");
}
int main()
{
   int i = 0x3412;
   Mem_rep((char *)&i, sizeof(i));
   return 0;
}
