#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main()
{
        int socket_desc;
        struct sockaddr_in server_addr;
        char server_message[2000], client_message[2000];   
        int server_struct_length = sizeof(server_addr);
        memset(server_message,'\0',sizeof(server_message));
        memset(client_message,'\0',sizeof(client_message));
        socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); 
        if(socket_desc < 0)
        {
                printf("Could Not Create Socket!\n");
                return -1;
        }
        //printf("Socket Created\n");
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(2000);
        server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        printf("Enter Chunck size (Number of char to recieve at a time): ");
        gets(client_message);
        if(sendto(socket_desc, client_message, strlen(client_message), 0, (struct sockaddr*)&server_addr, server_struct_length) < 0)
        {
                printf("Send Failed!\n");
                return -1;
        }
        int flag=0;
        while(flag==0)
        {
        	if(recvfrom(socket_desc, server_message, sizeof(server_message),0, (struct sockaddr*)&server_addr, &server_struct_length) < 0)
        	{
        	        printf("Receive Failed!\n");
        	        flag=1;
        	        return -1;
        	}
      		FILE* file = fopen("FileRecvd.txt", "a+");
			fputs(server_message,file);
        	memset(server_message,'\0',sizeof(server_message));
        	memset(client_message,'\0',sizeof(client_message));
        }
        fclose(file);
        close(socket_desc);
        return 0;
}

