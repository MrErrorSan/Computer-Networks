/*
        TCP_Server. This Program will will create the Server side for TCP_Socket Programming.
        It will receive the data from the client and then send the same data back to client.
*/
#include <stdio.h> 
#include <string.h> 
#include <sys/socket.h> //socket
#include <arpa/inet.h> //inet_addr
void *communicate(void *message)
{
        int client_sock = (int)message;
        char server_message[2000], client_message[2000];
        memset(server_message, '\0', sizeof(server_message));
        memset(client_message, '\0', sizeof(client_message));
        // Receive Name/CNIC from the client
        if (recv(client_sock, client_message, sizeof(client_message), 0) < 0)
        {
                printf("Receive Failed. Error!!!!!\n");
        }
        char credentials[255];
        strcpy(credentials, client_message);
        char *newline;
        newline = strchr(credentials, '\n');
        if (newline != NULL)
                *newline = '\0';
        //Checking for Voter identification
        FILE *fp;
        char buff[255];
        fp = fopen("Voters_List.txt", "r");
        int voter_found = 0;
        while (fgets(buff, sizeof(buff), fp) != NULL)
        {
                char *newline, *carriage_return;
                newline = strchr(buff, '\n');
                if (newline != NULL)
                        *newline = '\0';
                carriage_return = strchr(buff, '\r');
                if (carriage_return != NULL)
                        *carriage_return = '\0';
                if (strcmp(credentials, buff) == 0)
                {
                        voter_found = 1;
                        break;
                }
        }
        fclose(fp);
        // Checking for voter credibility
        if (voter_found == 1)
        {
                memset(server_message, '\0', sizeof(server_message));
                memset(client_message, '\0', sizeof(client_message));
                //Greeting message
                strcpy(server_message, "*** Welcome ***");
                if (send(client_sock, server_message, strlen(server_message), 0) < 0)
                {
                        printf("Send Failed. Error!!!!!\n");
                }
                fp = fopen("Casted_Votes.txt", "r");
                int vote_casted = 0;
                while (fgets(buff, 200, fp) != NULL)
                {
                        char *temp_comma;
                        temp_comma = strchr(buff, ',');
                        if (temp_comma != NULL)
                                *temp_comma = '\0';
                        if (strcmp(credentials, buff) == 0)
                        {
                                vote_casted = 1;
                                break;
                        }
                }
                fclose(fp);
                // vote not casted previously, so allow to cast vote
                if (vote_casted == 0)
                {
                        // display candidates list
                        fp = fopen("./Candidates_List.txt", "r");

                        char candidates[1000];

                        strcpy(candidates, "Following are the list of candidates and their symbols: \n");

                        while (fgets(buff, sizeof(buff), fp) != NULL)
                        {
                                strcat(candidates, buff);
                        }
                        fclose(fp);
                        // send candidates data back to client
                        memset(server_message, '\0', sizeof(server_message));
                        memset(client_message, '\0', sizeof(client_message));
                        strcpy(server_message, candidates);
                        if (send(client_sock, server_message, strlen(server_message), 0) < 0)
                        {
                                printf("Send Failed. Error!!!!!\n");
                        }
                        memset(server_message, '\0', sizeof(server_message));
                        memset(client_message, '\0', sizeof(client_message));
                        // Receive candidate's symbol from client
                        if (recv(client_sock, client_message, sizeof(client_message), 0) < 0)
                        {
                                printf("Receive Failed. Error!!!!!\n");
                        }
                        // write this vote in Casted_Votes.txt
                        char symbol[100];
                        strcpy(symbol, client_message);
                        char vote[200];
                        strcpy(vote, credentials);
                        strcat(vote, ",");
                        strcat(vote, symbol);
                        strcat(vote, "\n");
                        fp = fopen("Casted_Votes.txt", "a");
                        fputs(vote, fp);
                        fclose(fp);
                        // send a vote succesfully casted message back to client
                        memset(server_message, '\0', sizeof(server_message));
                        memset(client_message, '\0', sizeof(client_message));
                        strcpy(server_message, "Congratulations! Your vote has been succesfully casted.\n");
                        if (send(client_sock, server_message, strlen(server_message), 0) < 0)
                        {
                                printf("Send Failed. Error!!!!!\n");
                        }
                }
                else
                {
                        // send a vote casted failed message back to client
                        memset(server_message, '\0', sizeof(server_message));
                        memset(client_message, '\0', sizeof(client_message));
                        strcpy(server_message, "Sorry! You can only cast vote once.\n");
                        if (send(client_sock, server_message, strlen(server_message), 0) < 0)
                        {
                                printf("Send Failed. Error!!!!!\n");
                        }
                }
        }
        else
        {
                memset(server_message, '\0', sizeof(server_message));
                memset(client_message, '\0', sizeof(client_message));
                strcpy(server_message, "Sorry! Your name is not in the voters list.\n");
                if (send(client_sock, server_message, strlen(server_message), 0) < 0)
                {
                        printf("Send Failed. Error!!!!!\n");
                }
        }
        close(client_sock);
        pthread_exit(NULL);
}
int main(void)
{
        int socket_desc, client_sock, client_size; 
        struct sockaddr_in server_addr, client_addr;         //SERVER ADDR will have all the server address
        char server_message[2000], client_message[2000];                 // Sending values from the server and receive from the server we need this

        //Cleaning the Buffers
        
        memset(server_message,'\0',sizeof(server_message));
        memset(client_message,'\0',sizeof(client_message));     // Set all bits of the padding field//
        
        //Creating Socket
        
        socket_desc = socket(AF_INET, SOCK_STREAM, 0);
        
        if(socket_desc < 0)
        {
                printf("Could Not Create Socket. Error!!!!!\n");
                return -1;
        }
        
        printf("Socket Created\n");
        
        //Binding IP and Port to socket
        
        server_addr.sin_family = AF_INET;               /* Address family = Internet */
        server_addr.sin_port = htons(2000);               // Set port number, using htons function to use proper byte order */
        server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");    /* Set IP address to localhost */
		
		
		
		// BINDING FUNCTION
        
        if(bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr))<0)    // Bind the address struct to the socket.  /
	                            	//bind() passes file descriptor, the address structure,and the length of the address structure
        {
                printf("Bind Failed. Error!!!!!\n");
                return -1;
        }        
        
        printf("Bind Done\n");
        
        //Put the socket into Listening State
        
        if(listen(socket_desc, 1) < 0)                               //This listen() call tells the socket to listen to the incoming connections.
     // The listen() function places all incoming connection into a "backlog queue" until accept() call accepts the connection.
        {
                printf("Listening Failed. Error!!!!!\n");
                return -1;
        }
        
        printf("Listening for Incoming Connections.....\n");
        while(1)
        {
                //Accept the incoming Connections
                client_size = sizeof(client_addr);
                client_sock = accept(socket_desc, (struct sockaddr*)&client_addr, &client_size);// heree particular client k liye new socket create kr rhaa ha
                
                if (client_sock < 0)
                {
                        printf("Accept Failed. Error!!!!!!\n");
                        return -1;
                }
        
                printf("Client Connected with IP: %s and Port No: %i\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
		       //inet_ntoa() function converts the Internet host address in, given in network byte order, to a string in IPv4 dotted-decimal notation
        
                //Receive the message from the client
        
                if (recv(client_sock, client_message, sizeof(client_message),0) < 0)
                {
                        printf("Receive Failed. Error!!!!!\n");
                        return -1;
                }
        
                printf("Client Message: %s\n",client_message);
        
                //Send the message back to client
        
                strcpy(server_message, client_message);
        
                if (send(client_sock, server_message, strlen(client_message),0)<0)
                {
                        printf("Send Failed. Error!!!!!\n");
                        return -1;
                }
        
                memset(server_message,'\0',sizeof(server_message));
                memset(client_message,'\0',sizeof(client_message));
                //##############################################################
                pthread_t thread1;
                // create new thread for each client
                pthread_create(&thread1, NULL, communicate, (void *)client_sock);
        }    
        //Closing the Socket
        close(client_sock);
        close(socket_desc);
        return 0;       
}
