#include <stdio.h>
#include <string.h>
#include <sys/socket.h>      //socket
#include <arpa/inet.h>       //inet_addr
#include<stdbool.h>
#include<pthread.h>      

int busNo = 2000;
int seatNo = 0;
char City1[50] = "city ISLAMABAD";
char City2[50] = "city FAISALABAD ";

int ActiveClient = 0;
int ClientIndex = 0;
int Clients[3] = {0,0,0};

bool Compare(char arr1[200], char arr2[2000], int start)
{
	int i = 0;
	while(i < sizeof(arr1))
	{
		if(arr1[i] != arr2[start])
		{
			return false;
		}
		i++;
		start++;
	}
	return true;
}

bool FindTheSchedule(char client_message[2000], char server_message[2000], char FileName[50])
{

	FILE *fptr = fopen(FileName, "r");
	int start;
	while(!feof(fptr))
	{
		char Dummy[200];
		int index = 0;
		int i = 0;
		char Temp = fgetc(fptr);
		start = index;
		while(!feof(fptr) && Temp != '\n')
		{
			Dummy[i++] = Temp;
			index++;
			Temp = fgetc(fptr);
		}
		
		if(Compare(Dummy, client_message, start))
		{
			FILE* fptr1 = fopen("booking-final.txt","r");
			char Data[200];
			strcpy(server_message, "Your seat Number is ---, and bus Number is --- \n");
			return true;
		}
		else
		{
				strcpy(server_message, "Nothing Found");
				return false;
		}
	}
}

void ThreadFunction(void* buffer)
{
	struct sockaddr_in server_addr, client_addr;
    char server_message[2000], client_message[2003];
    int *client_socket = (int *)buffer;
    int client_sock = *client_socket;
    
	int client_Index = ClientIndex;
    Clients[ClientIndex] = 1;
	int i;
    for ( i = 0; i < 3; i++)
    {
        if (Clients[i] == 0)
        {
            ClientIndex = 1;
            break;
        }
    }


   while (1)
    {

        if (recv(client_sock, client_message, sizeof(client_message), 0) < 0)
        {
            printf("Receive Failed. Error!!!!!\n");
            pthread_exit(NULL);
        }

        if (strcmp(client_message, "-1") == 0)
        {   
			break;
		}
		printf("Client Message: %s\n",  client_message);
		

		memset(server_message, '\0', sizeof(server_message));
		memset(client_message, '\0', sizeof(client_message));
		
		strcpy(server_message, "Enter the City Name ");
		//gets(client_message);
		if (send(client_sock, server_message, strlen(client_message), 0) < 0)
		{
			printf("Send Failed. Error!!!!!\n");
			pthread_exit(NULL);
		}
		if (recv(client_sock, client_message, sizeof(client_message), 0) < 0)
		{
			printf("Receive Failed. Error!!!!!\n");
			pthread_exit(NULL);
		}
		if(strcmp(client_message,"city ISLAMABAD" ) == 0)
		{
			memset(client_message, '\0', sizeof(client_message));
			memset(server_message, '\0', sizeof(server_message));
			if (send(client_sock, server_message, strlen(client_message), 0) < 0)
			{
				printf("Send Failed. Error!!!!!\n");
				pthread_exit(NULL);
			}
			if (recv(client_sock, client_message, sizeof(client_message), 0) < 0)
			{
				printf("Receive Failed. Error!!!!!\n");
				pthread_exit(NULL);
			}
				FindTheSchedule(client_message, server_message, "city ISLAMABAD.txt");
			if (send(client_sock, server_message, strlen(client_message), 0) < 0)
			{
				printf("Send Failed. Error!!!!!\n");
				pthread_exit(NULL);
			}
		}
		else if(strcmp(client_message,"city FAISALABAD" ) == 0)
		{
			memset(server_message, '\0', sizeof(server_message));
			memset(client_message, '\0', sizeof(client_message));
			if (send(client_sock, server_message, strlen(client_message), 0) < 0)
			{
				printf("Send Failed. Error!!!!!\n");
				pthread_exit(NULL);
			}
			if (recv(client_sock, client_message, sizeof(client_message), 0) < 0)
			{
				printf("Receive Failed. Error!!!!!\n");
				pthread_exit(NULL);
			}
			FindTheSchedule(client_message, server_message," city FAISALABAD.txt");
			if (send(client_sock, server_message, strlen(client_message), 0) < 0)
			{
				printf("Send Failed. Error!!!!!\n");
				pthread_exit(NULL);
			}
		}else
		{
			memset(server_message, '\0', sizeof(server_message));
			memset(client_message, '\0', sizeof(client_message));
			strcpy(server_message,"Nothing Found");
		}
		if (send(client_sock, server_message, strlen(client_message), 0) < 0)
		{
			printf("Send Failed. Error!!!!!\n");
			pthread_exit(NULL);
		}
	}
	close(client_sock);
	Clients[client_Index] = 0;
	ActiveClient--;
	ClientIndex = client_Index;
}
void *ConnectionRefuser(void *message)
{
    struct sockaddr_in server_addr, client_addr;
    char server_message[2000], client_message[2000];
    int *client_socket = (int *)message;
    int client_sock = *client_socket;
    

    strcpy(server_message, "Server is Full");

    send(client_sock, server_message, strlen(server_message), 0);

    close((*client_socket));
    pthread_exit(NULL);
}
int main()
{
	int socket_desc, client_sock, client_size;
    struct sockaddr_in server_addr, client_addr;     
    char server_message[2000], client_message[2000]; 
	pthread_t threads[3];
    int Clientsockets[4];
    int thread_index = 0;
    //Cleaning the Buffers

    memset(server_message, '\0', sizeof(server_message));
    memset(client_message, '\0', sizeof(client_message)); 
    //Creating Socket

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

    
    if (bind(socket_desc, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) 
	{
        printf("Bind Failed. Error!!!!!\n");
        return -1;
    }

    printf("Bind Done\n");

    
    if (listen(socket_desc, 1) < 0) 
	{
        printf("Listening Failed. Error!!!!!\n");
        return -1;
    }

    printf("Listening for Incoming Connections.....\n");
    client_size = sizeof(client_addr);
    //Accept the incoming Connections
    while (1)
    {

        Clientsockets[thread_index] = accept(socket_desc, (struct sockaddr *)&client_addr, &client_size);
		if (client_sock < 0)
        {
            printf("Accept Failed. Error!!!!!!\n");
            return -1;
        }
        if (ActiveClient < 3)
        {
            printf("Thread Created\n");
            ActiveClient++;
            pthread_create(&threads[thread_index], NULL,ThreadFunction, (void*)&Clientsockets[thread_index]);
            thread_index++;
        }
        else
        {
            printf("Fourth Thread Created\n");
            pthread_create(&threads[thread_index], NULL, ConnectionRefuser, (void*)&Clientsockets[thread_index]);
        }
    }

    memset(server_message, '\0', sizeof(server_message));
    memset(client_message, '\0', sizeof(client_message));

    // close(client_sock);
    close(socket_desc);
    return 0;
}
