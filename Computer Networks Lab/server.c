#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
int lenArr(char *a)
{
	int size=0;
	for(;a[size]!=0;size++){}
	return size;
}
int charToInt(char* a)
{
	int size=0;
	int sum=0;
	int mul=1;
	for(;a[size]!=0;size++){}
	for(int i=size-1;i>0;i--)
	{
		int x=a[i]-48;
		x=x*mul;
		sum= sum+x;
		mul=mul*10;
	}
	return sum;
}
int main(void)
{
        int socket_desc;
        struct sockaddr_in server_addr, client_addr;
        char server_message[2000], client_message[2000]; 
        int client_struct_length = sizeof(client_addr);
        memset(server_message,'\0',sizeof(server_message));
        memset(client_message,'\0',sizeof(client_message));
        socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        
        if(socket_desc < 0)
        {
                printf("Could Not Create Socket!\n");
                return -1;
        }
        printf("Socket Created\n");
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(2000);
        server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
        if(bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr))<0)
        {
                printf("Bind Failed. Error!!!!!\n");
                return -1;
        }               
        printf("Bind Done\n");
        while (1)			//server always running 
        {
        	printf("Listening for Chunck Size...\n\n");
        	if (recvfrom(socket_desc, client_message, sizeof(client_message), 0, (struct sockaddr*)&client_addr,&client_struct_length) < 0)
        	{
           	     printf("Receive Failed!\n");
           	     return -1;
        	}
      		FILE* file = fopen("FileSend.txt", "r");
      		char arr[5000];
			fgets(arr,5000,file);
			fclose(file);
			int index=0;
			int n= lenArr(arr)/charToInt(client_message);
			for(int i=0;i<n;i++)
			{
				int chunck=charToInt(client_message);
				int y= (i*chunck)+1;
				index=0;
				for(int j=y;j<(y+chunck);j++)
				{
					server_message[index]=arr[j];
					index++;
				}
				server_message[index]='\0';
        		if (sendto(socket_desc, server_message, strlen(server_message), 0, (struct sockaddr*)&client_addr,client_struct_length)<0)
        		{
        	    	    printf("Send Failed!\n");
        	    	    return -1;
        		}
        	
        		memset(server_message,'\0',sizeof(server_message));
        		memset(client_message,'\0',sizeof(client_message));
        	}
        }  
        close(socket_desc);
        return 0;       
}
