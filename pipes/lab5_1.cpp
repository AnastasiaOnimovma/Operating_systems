#include <pthread.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
 
const int SIZE=256;
int pipefd[2];

static void* write(void* flag){
    std::cout<<"\nthread 1 started working\n";
    int* lc_flag=(int*)flag;
    hostent *host;
    char buf[SIZE]="www.google.ru";
    while(*lc_flag){
    	host = gethostbyname(buf);
    	strcpy(buf,inet_ntoa(*(in_addr*)host->h_addr));
   		ssize_t nw=write(pipefd[1],buf,SIZE); 
   		if(nw==-1){
   			perror("write");
   			sleep(1);
   		}
   		else if(nw==0){
   			std::cout<<std::flush<<"file end";
   		}
   		else{
   			sleep(1);
   		}  
    }
    std::cout<<"\nthread 1 finished working\n";
    pthread_exit((void**)1);
    
}
static void* read(void* flag){
	std::cout<<"\nthread 2 started working\n";
    int* lc_flag=(int*)flag;
    char buf[SIZE];
    while(*lc_flag){
    	memset(buf,0,SIZE);
   		ssize_t nr=read(pipefd[0],buf,SIZE);
   		if(nr==-1){
   			perror("read");
   			sleep(1);
   		}
   		else if(nr==0){
   			std::cout<<std::flush<<"file end";
   		}
   		else{
   			std::cout<<buf<<"\n";
   		}
    }
    std::cout<<"\nthread 2 finished working";
    pthread_exit((void**)2);
    
}


int main() {
    std::cout<<"program started\n";
    pthread_t id1, id2;
    int flag1=1, flag2=1;
    int rv=pipe(pipefd);
     if (rv == -1) {
        perror("pipe");
        exit(1);
    }
    int *exitcode1=new int, *exitcode2=new int;
    pthread_create(&id1, NULL, write, (void*)(&flag1));
    pthread_create(&id2, NULL, read, (void*)(&flag2));
    std::cout<<"\npress any key to stop\n";
    getchar();
    std::cout<<"\nkey is pressed\n";
    flag1 = 0;
    flag2 = 0;
    pthread_join(id1, (void**)exitcode1);
    std::cout<<"\nwrite func finished with "<<*exitcode1<<"\n";
    pthread_join(id2, (void**)exitcode2);
    std::cout<<"\nread func finished with "<<*exitcode2<<"\n";
    delete exitcode1, exitcode2;
    close(pipefd[0]);
    close(pipefd[1]);
    std::cout<<"\nprogram finished\n";
    return 0;
}
