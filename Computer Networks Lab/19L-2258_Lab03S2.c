#include <sys/socket.h> //socket
#include <arpa/inet.h> //inet_addr
#include <stdio.h> 
#include <string.h> 
int isvowel(char a){
	return (a=='a'||a=='e'||a=='i'||a=='O'||a=='u'||a=='A'||a=='E'||a=='I'||a=='O'||a=='U');
}
int isWantedword(char word[]){
	int i=0;
	for(;i<strlen(word);i++){
		if(isvowel(word[i])){
			return 1;
		}
	}
	return 0;
}
void rev(char str[]){
	char temp[100];
	memset(temp,'\0',100);
	int t=0;
	int i=strlen(str)-1;
	for(;i>=0;i--){
		temp[t++]=str[i];	
	}
	i=0;
	for(i=0;i<strlen(temp)+1;i++)
		str[i]=temp[i];	
} 
void reverse(char str[]){
	char temp[100];
	int t=0;
	int i=0;
	for(;i<=strlen(str);i++){
		if(str[i]==' '||str[i]=='\0'){
			temp[t++]='\0';
			t=0;
			if(isWantedword(temp)){
				rev(temp);	
				int j=i-strlen(temp);	
				for(;j<i;j++){
					str[j]=temp[t++];			
				}
			t=0;
			}
			
		}
		else{
			temp[t++]=str[i];		
		}
	}

}
int main(void)
{
        int socket_desc, client_sock, client_size; 
        struct sockaddr_in server_addr, client_addr;         //SERVER ADDR will have all the server address
        char server_message[2000], client_message[2000];                 // Sending values from the server and receive from the server we need this
        memset(server_message,'\0',sizeof(server_message));
        memset(client_message,'\0',sizeof(client_message));     // Set all bits of the padding field//
        socket_desc = socket(AF_INET, SOCK_STREAM, 0);
        if(socket_desc < 0)
        {
                printf("Could Not Create Socket. Error!!!!!\n");
                return -1;
        }
        printf("Socket Created\n");
        server_addr.sin_family = AF_INET;               /* Address family = Internet */
        server_addr.sin_port = htons(2000);               // Set port number, using htons function to use proper byte order */
        server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");    /* Set IP address to localhost */
        if(bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr))<0)    // Bind the address struct to the socket.  /
        {
                printf("Bind Failed. Error!!!!!\n");
                return -1;
        }        
        printf("Bind Done\n");
        if(listen(socket_desc, 1) < 0)
        {
                printf("Listening Failed. Error!!!!!\n");
                return -1;
        }
        
        printf("Listening for Incoming Connections.....\n");
        client_size = sizeof(client_addr);
        client_sock = accept(socket_desc, (struct sockaddr*)&client_addr, &client_size); 
        
        if (client_sock < 0)
        {
                printf("Accept Failed. Error!!!!!!\n");
                return -1;
        }
        if (recv(client_sock, client_message, sizeof(client_message),0) < 0)
        {
                printf(" Error Receive Failed.!!!!!\n");
                return -1;
        }
        reverse(client_message);
        printf("Client Message: %s\n",client_message);
        strcpy(server_message, client_message);
        if (send(client_sock, server_message, strlen(server_message),0)<0)
        {
                printf("Send Failed. Error!!!!!\n");
                return -1;
        }
        memset(server_message,'\0',sizeof(server_message));
        memset(client_message,'\0',sizeof(client_message));
        close(client_sock);
        close(socket_desc);
        return 0;       
}
