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
#include <sys/un.h>
#include <netdb.h>


int server_fd; 
int client_fd;
sockaddr_un address,cl_address; 
std::vector <std::string> msglist; 
pthread_t id1, id2,id3;
pthread_mutex_t mut_qw, mut_an;
int flag1, flag2, flag3;

static void* accept_request(void *) {
	std::cout<<"\naccepting started\n";
	char buf[256];
	std::string str;
    while(flag2){
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
			if(pthread_mutex_trylock(&mut_qw)==0){
				str=buf;
				msglist.push_back(str);
				pthread_mutex_unlock(&mut_qw);
				std::cout<<"\nrequest received: "<<str<<"\n";
			}
					
		}
	}

    pthread_exit((void**)2);
}

static void* process_request(void *) {
	std::cout<<"\nprocessing started\n";
	hostent *host;
	char buf[256];
    while(flag3){
    	if(pthread_mutex_trylock(&mut_an)==0){
    		if (!msglist.empty()) {
				std::string str = msglist.back();
				msglist.pop_back();
				pthread_mutex_unlock(&mut_an);
				host = gethostbyname(&str[0]); 
    			strcpy(buf,inet_ntoa(*(in_addr*)host->h_addr));
    			str= str+" " + buf;
    			int sentcount = send(client_fd, str.c_str(), str.length(), 0);
				if (sentcount == -1) {
					perror("send");
				}
				else{
					std::cout<<"message sent: "<<str<<"\n";
				}
			}
			else{
				pthread_mutex_unlock(&mut_an);
				sleep(1);
			}
    	}
    	
	}
    pthread_exit((void**)3);
}

static void* wait_connect(void *) {
	std::cout<<"\nwaiting for connection started\n";
	socklen_t size=sizeof(address);
    while(flag1){
    	client_fd = accept(server_fd,(struct sockaddr*)&address, &size);
    	if (client_fd == -1) {
			perror("accept");
			sleep(1);
		}
		else{
			flag2=1;
			flag3=1;
			std::cout<<"\nconnection is established\n";
			pthread_create(&id2, NULL, accept_request,NULL);
			pthread_create(&id3, NULL, process_request,NULL);
			flag1=0;
    	}
	}
	pthread_exit((void**)1);
}

int main() {
	std::cout<<"program started\n";
	int opt=1;
	address.sun_family = AF_UNIX;
	strcpy(address.sun_path,"mysocket");
	server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	fcntl(server_fd, F_SETFL, O_NONBLOCK);
	socklen_t size=sizeof(address);
	if(bind(server_fd, (struct sockaddr*)&address,size)==-1){
		perror("bind");
	}
	bind(server_fd, (struct sockaddr*)&address,size);
	listen(server_fd, 20);
	msglist= {};
	flag1=1;
	int *exitcode1=new int,*exitcode2=new int,*exitcode3=new int;
	pthread_mutex_init(&mut_qw, NULL);
	pthread_mutex_init(&mut_an, NULL);
	pthread_create(&id1, NULL, wait_connect,NULL); 	
	std::cout<<"\npress any key to stop\n";
    getchar();
    std::cout<<"\nkey is pressed\n";
    flag1=0;
	flag2 = 0;
	flag3 = 0;
	pthread_join(id1, (void**)exitcode1);
	pthread_join(id2, (void**)exitcode2);
	pthread_join(id3, (void**)exitcode3);
	shutdown(client_fd, SHUT_RDWR);
	close(client_fd);
	close(server_fd);
	std::cout<<"\nwaiting for connection finished with "<<*exitcode1<<"\n";
	std::cout<<"\naccepting finished with "<<*exitcode2<<"\n";;
	std::cout<<"\nprocessing finished with "<<*exitcode3<<"\n";;
	delete exitcode1,exitcode2,exitcode3;
	pthread_mutex_destroy(&mut_qw);
	pthread_mutex_destroy(&mut_an);
	std::cout<<"\nprogram finished\n";
	return 0;
}


