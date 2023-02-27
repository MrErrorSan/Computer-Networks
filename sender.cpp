#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<iostream>
int main(){

int res,n;
res=open("fifo1",O_WRONLY);
write(res,"Message",7);
//printf("Sender Process with pid %d send a message", getpid());
cout<<"Sender process with pid "<<getpid()<<" send a message"<<endl;
return 0;
}
