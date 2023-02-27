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
			return 0;
		}
	}
	return 1;
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
        int socket_desc;
        struct sockaddr_in server_addr;
        char server_message[2000], client_message[2000];
        memset(server_message,'\0',sizeof(server_message));
        memset(client_message,'\0',sizeof(client_message));
        socket_desc = socket(AF_INET, SOCK_STREAM, 0);
        if(socket_desc < 0)
        {
                printf("Could Not Create Socket. Error!!!!!\n");
                return -1;
        }
        printf("Socket Created\n");
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(2000);
        server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        if(connect(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
        {
                printf("Connection Failed. Error!!!!!");
                return -1;
        }
        printf("Connected\n");
        printf("Enter Message: ");
        gets(client_message);
        if(send(socket_desc, client_message, strlen(client_message),0) < 0)
        {
                printf("Send Failed. Error!!!!\n");
                return -1;
        }
        if(recv(socket_desc, server_message, sizeof(server_message),0) < 0)
        {
                printf("Receive Failed. Error!!!!!\n");
                return -1;
        }
	reverse(server_message);
        printf(": %s\n",server_message);  
        memset(server_message,'\0',sizeof(server_message));
        memset(client_message,'\0',sizeof(client_message));
        close(socket_desc);
        return 0;
}
