#include <iostream>
#include <pthread.h>
#include <fcntl.h> 
#include <unistd.h>
#include <iomanip> 
#include <cstring>
#include <sys/mman.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

const int SIZE=256;
int msgid;

struct Message{
	long mtype;
	char buf[SIZE];
};

static void* writing(void* flag){
	std::cout<<"\nwriting started\n";
	int* lc_flag=(int*)flag;
	hostent *host;
	Message message;
    while(*lc_flag){
    	host = gethostbyname("ru.stackoverflow.com");
    	message.mtype=1;
    	strcpy(message.buf,inet_ntoa(*(in_addr*)host->h_addr));
		std::cout<<message.buf<<"\n";
		int result = msgsnd(msgid,&message,sizeof(message), IPC_NOWAIT);
		if (result ==-1){
			perror("msgsnd");
		}
		sleep(1);
    }
    pthread_exit((void**)1);
}

int main(){
	std::cout<<"program started\n";
	pthread_t id1;
	int flag=1;
	int *exitcode=new int;
	key_t key = ftok("lab7_1.cpp",'A');
	msgid= msgget(key,0);
	if (msgid<0){
		msgid=msgget(key,IPC_CREAT|0644);
	}
    pthread_create(&id1, NULL, writing, (void*)(&flag));
	std::cout<<"\npress any key to stop\n";
    getchar();
    std::cout<<"\nkey is pressed\n";
    flag = 0;
	pthread_join(id1, (void**)exitcode);
	msgctl(msgid, IPC_RMID, NULL);
    std::cout<<"\nwriting finished with:"<<*exitcode<<"\n";
	std::cout<<"\nprogram finished\n";
	return 0;
}
