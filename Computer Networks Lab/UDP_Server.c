/*
        UDP_Server. This Program will will create the Server side for UDP_Socket Programming.
        It will receive the data from the client and then send the same data back to client.
*/

#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
#include <sys/socket.h> //// Needed for socket creating and binding
#include <arpa/inet.h> //inet_addr

int main(void)
{
        int socket_desc;
        struct sockaddr_in server_addr, client_addr;
        char server_message[2000], client_message[2000]; 
        int client_struct_length = sizeof(client_addr);
        
        //Cleaning the Buffers
        
        memset(server_message,'\0',sizeof(server_message));
        memset(client_message,'\0',sizeof(client_message));
        
        //Creating UDP Socket
        
        socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        
        if(socket_desc < 0)
        {
                printf("Could Not Create Socket. Error!!!!!\n");
                return -1;
        }
        
        printf("Socket Created\n");
        
        //Binding IP and Port to socket
        
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(2000);
        server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  // bind your socket to localhost only, if you want connect any particular ip you should mention it in INET_ADDR.   
        if(bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr))<0)
        {
                printf("Bind Failed. Error!!!!!\n");
                return -1;
        }               
        printf("Bind Done\n");
        //######################################################################
        FILE* file = fopen("file.txt", "r");
		int size = 0;
		fscanf(file,"%d",&size);      
        char *strs[20];		
		char temp_roll[15];
		int l = 0;
		for(;l<size;l++)
		{
			fscanf(file, "%s", temp_roll);        		
			strs[l] = malloc(strlen(temp_roll+1));
			strcpy(strs[l], temp_roll);
		}
		fclose(file);
        while (1)
        {
        //######################################################################
        	printf("Listening for Messages...\n\n");
        	//Receive the message from the client
        	if (recvfrom(socket_desc, client_message, sizeof(client_message), 0, (struct sockaddr*)&client_addr,&client_struct_length) < 0)
        	{
           	     printf("Receive Failed. Error!!!!!\n");
           	     return -1;
        	}
        	printf("Received Message from IP: %s and Port No: %i\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
        //######################################################################
        	//Searching for Rollno
        	char rollno[15];
			strcpy(rollno, client_message);
			rollno[7]='\0';
			int i = 0;
			int found = 0;
			int f_index = 0;
			for (i = 0; i < size; i++)
			{
				if(strcmp(strs[i], rollno) == 0)
				{
					found = 1;
					f_index = i;
					break;
				}
			}
			//Conditions for CheckIn
			if(client_message[8]=='C' && client_message[9]=='I')
			{
				if(found==0)
				{
					strs[size] = malloc(strlen(rollno) + 1);
					strcpy(strs[size], rollno);
					size++;
					strcpy(server_message, "Welcome Student ");
					strcat(server_message, rollno);
					strcat(server_message, " !\n");
					file = fopen("file.txt", "w");
					char a_size[5];
					snprintf (a_size, sizeof(a_size), "%d",size);
					fputs (a_size, file);
					fputs (" ", file);
					int k = 0;
					for(; k < size; k++)
					{
						fputs(strs[k], file);
						fputs (" ", file);
					}
					fclose(file);
				}else
				{
					strcpy(server_message, "You are already here.\n");
				}
			//Conditions for CheckOut	
			}else if(client_message[8]=='C' && client_message[9]=='O')
			{
				if (found == 1)
				{
					int j = f_index;
					for(; j < size - 1;j++)		//arranging file output
					{
						strcpy(strs[j], strs[j+1]);
					}
					free (strs[size - 1]);
					size--;					
					strcpy(server_message, "Goodbye Student ");
					strcat(server_message, rollno);
					strcat(server_message, " ! Have a nice day!\n");
					file = fopen("file.txt", "w");
					char a_size[5];
					snprintf (a_size, sizeof(a_size), "%d",size);
					fputs (a_size, file);
					fputs (" ", file);
					int k = 0;
					for(; k < size; k++)
					{
						fputs(strs[k], file);
						fputs (" ", file);
					}
					fclose(file);
				}
				else
				{
					strcpy(server_message, "You didn't check in today. Contact System Administrator.\n");
				}
			}else
				printf("ERROR!!!!!!\n");
			if (size > 0)
			{			
				printf("Following students are present:-\n");
				int c = 0;
				for(; c < size; c++)
				{
					printf("%s",strs[c]);
					printf("\n");
				}
			}
			else
				printf("No student is present.\n");
			
		//######################################################################
	        //printf("Client Message: %s\n",client_message);
	        //Send the message back to client
	        //strcpy(server_message, client_message);
        	if (sendto(socket_desc, server_message, strlen(server_message), 0, (struct sockaddr*)&client_addr,client_struct_length)<0)
        	{
            	    printf("Send Failed. Error!!!!!\n");
            	    return -1;
        	}
        
        	memset(server_message,'\0',sizeof(server_message));
        	memset(client_message,'\0',sizeof(client_message));
        }  
        //Closing the Socket
        close(socket_desc);
        return 0;       
}
