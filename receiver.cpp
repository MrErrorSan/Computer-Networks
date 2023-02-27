#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<iostream>
int main(){

int res,n;
char buffer[100];
res=open("mypipe",O_RDONLY);
n=read(res,buffer,100);
//printf("Reader Process having pid %d has started",getpid());
cout<<"Reader Process having pid : \""<<getpid()<<"\" has started"<<endl;
//printf("Data received by Reciver %d is %s", getpid(), buffer);
cout<<"Data received by Reciver "<<getpid()<<" is "<<buffer<<endl;
}
