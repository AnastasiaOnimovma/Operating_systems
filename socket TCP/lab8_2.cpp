#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <vector>
#include <string>
#include <iostream>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/un.h>


int client_fd;
sockaddr_un serv_address; 
pthread_t id1, id2,id3;
int flag1, flag2, flag3;

void* send_request(void*) {
	std::cout<<"\nsending started\n";
	//char buf[256];
	std::string buf;
    while(flag2){
    	buf="etu.ru";
		int sendcount= send(client_fd, buf.c_str(), sizeof(buf),0);
		if (sendcount == -1) {
			perror("send");
		}
		else{
			std::cout<<"\nrequest sent: "<<buf<<"\n";	
		}
		sleep(1);
	}

    pthread_exit((void**)2);
}

void* receive_response(void*) {
	std::cout<<"\nreceiving responses started\n";
	char buf[256];
	std::string str;
    while(flag3){
    	int reccount = recv(client_fd, &buf, sizeof(buf),0);
		if (reccount == -1) {
			perror("recv");
			sleep(1);
		}
		else if (reccount == 0) {
			shutdown(client_fd, SHUT_RDWR);
			sleep(1);
		}
		else{
			str=buf;
			std::cout<<"message received: "<<str<<"\n";		
		}
	}
    pthread_exit((void**)3);
}

void* wait_connect(void *) {
	std::cout<<"\nconnection establishing started\n";
	socklen_t size=sizeof(serv_address);
    while(flag1){
    int result = connect(client_fd,(struct sockaddr*)&serv_address,size);
    if (result == -1) {
		perror("connect");
		sleep(1);
	}
	else{
		flag2=1;
		flag3=1;
		std::cout<<"\nconnection is established\n";
		pthread_create(&id2, NULL, send_request, 0);
		pthread_create(&id3, NULL, receive_response, 0);
		flag1=0;
    	}
	}
	pthread_exit((void**)1);
}

int main() {
	std::cout<<"program started\n";
	int opt=1;
	serv_address.sun_family = AF_UNIX;
	strcpy(serv_address.sun_path,"mysocket");
	client_fd = socket(AF_UNIX, SOCK_STREAM, 0);
	flag1=1;
	int *exitcode1=new int,*exitcode2=new int,*exitcode3=new int;
	pthread_create(&id1, NULL, wait_connect, 0); 	
	std::cout<<"\npress any key to stop\n";
    getchar();
    std::cout<<"\nkey is pressed\n";
    flag1=0;
	flag2 = 0;
	flag3 = 0;
	pthread_join(id1, (void**)exitcode1);
	pthread_join(id2, (void**)exitcode2);
	pthread_join(id3, (void**)exitcode3);
	//shutdown(client_fd, SHUT_RDWR);
	close(client_fd);
	std::cout<<"\nconnection establishing with "<<*exitcode1<<"\n";
	std::cout<<"\naccepting finished with "<<*exitcode2<<"\n";
	std::cout<<"\nprocessing finished with "<<*exitcode3<<"\n";
	delete exitcode1,exitcode2,exitcode3;
	std::cout<<"\nprogram finished\n";
	return 0;
}



