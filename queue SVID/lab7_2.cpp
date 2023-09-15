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

static void* reading(void* flag){
	std::cout<<"\nreading started\n";
	int* lc_flag=(int*)flag;
	Message message;
    while(*lc_flag){
    	message.mtype=1;
    	memset(message.buf,0,SIZE);
    	int result = msgrcv(msgid,&message,sizeof(message),message.mtype,IPC_NOWAIT);
    	if(result==-1){
    		perror("msgrcv");
    		sleep(3);
    		continue;
    	}
		std::cout<<message.buf<<"\n";
    }
    pthread_exit((void**)2);
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
    pthread_create(&id1, NULL, reading, (void*)(&flag));
	std::cout<<"\npress any key to stop\n";
    getchar();
    std::cout<<"\nkey is pressed\n";
    flag = 0;
	pthread_join(id1, (void**)exitcode);
	msgctl(msgid, IPC_RMID, NULL);
    std::cout<<"\nreading finished with:"<<*exitcode<<"\n";
	std::cout<<"\nprogram finished\n";
	return 0;
}
