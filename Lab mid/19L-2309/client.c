#include <stdio.h>
#include <string.h>
#include <sys/socket.h>      //socket
#include <arpa/inet.h>       //inet_addr
#include<stdbool.h>
#include<pthread.h>  

int main()
{
	int socket_desc;
	struct sockaddr_in server_addr;
	char server_message[2000], client_message[2000];

	//Cleaning the Buffers

	memset(server_message, '\0', sizeof(server_message));
	memset(client_message, '\0', sizeof(client_message));

	//Creating Socket
	// while (1)
	// {
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);

	if (socket_desc < 0)
	{
			printf("Could Not Create Socket. Error!!!!!\n");
			return -1;
	}

	printf("Socket Created\n");

	

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(5002);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	
	if (connect(socket_desc, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
			printf("Connection Failed. Error!!!!!");
			return -1;
	}

	if (recv(socket_desc, server_message, sizeof(server_message), 0) < 0)
	{
			printf("Receive Failed. Error!!!!!\n");
			return -1;
	}

	printf("Server Message: %s\n", server_message);
	if (strcmp(server_message, "Connected") == 0)
	{
		while (1)
		{
			printf("Enter Message: ");
			gets(client_message); 

			if (send(socket_desc, client_message, strlen(client_message), 0) < 0)
			{
					printf("Send Failed. Error!!!!\n");
					return -1;
			}
			if (strcmp(client_message, "-1") == 0)
			{
					printf("Connection is closed\n");
					break;
			
			memset(server_message, '\0', sizeof(server_message));
			if (recv(socket_desc, server_message, sizeof(server_message), 0) < 0)
			{
					printf("Receive Failed. Error!!!!!\n");
					return -1;
			}
			memset(client_message, '\0', sizeof(client_message));
			printf("Server Message: %s\n", server_message);
			gets(client_message);
			memset(server_message, '\0', sizeof(server_message));
			
			if (send(socket_desc, client_message, strlen(client_message), 0) < 0)
			{
					printf("Send Failed. Error!!!!\n");
					return -1;
			}	
		}
	}

   
	//Closing the Socket
	memset(server_message, '\0', sizeof(server_message));
	memset(client_message, '\0', sizeof(client_message));
	close(socket_desc);

	return 0;
}
