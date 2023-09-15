#include <pthread.h>
#include <iostream>
#include <unistd.h>
#include <time.h>
 
pthread_mutex_t mut;

static void* func1(void* flag){
    std::cout<<"\nthread 1 started working\n";
	int* lc_flag=(int*)flag;
	timespec tp;
	int time= clock_gettime(CLOCK_REALTIME,&tp);
	tp.tv_sec+=1;
    while(*lc_flag){
   		while(pthread_mutex_timedlock(&mut,&tp)==ETIMEDOUT){
			time= clock_gettime(CLOCK_REALTIME,&tp);    	
			tp.tv_sec+=1;
   		}
    	for(int i=0;i<10 && *lc_flag;i++){
    		std::cout<<std::flush;
    		std::cout<<"1";
    		sleep(1);
    	}
    	pthread_mutex_unlock(&mut);
    	sleep(1);
    }
    std::cout<<"\nthread 1 finished working\n";
    pthread_exit((void**)1);
    
}
static void* func2(void* flag){
    std::cout<<"\nthread 2 started working\n";
	int* lc_flag=(int*)flag;
	timespec tp;
	int time= clock_gettime(CLOCK_REALTIME,&tp);
	tp.tv_sec+=1;
    while(*lc_flag){
		while(pthread_mutex_timedlock(&mut,&tp)==ETIMEDOUT){
			time = clock_gettime(CLOCK_REALTIME,&tp);    	
			tp.tv_sec+=1;
   		}
    	for(int i=0;i<10 && *lc_flag;i++){
    		std::cout<<std::flush;
    		std::cout<<"2";
    		sleep(1);
    	}
    	pthread_mutex_unlock(&mut);
    	sleep(1);
    }
    std::cout<<"\nthread 2 finished working";
    pthread_exit((void**)2); 
}


int main() {
    std::cout<<"program started\n";
    pthread_t id1, id2;
    pthread_mutex_t mut;
    int flag1=1, flag2=1;
    pthread_mutex_init(&mut, NULL);
    int *exitcode1=new int, *exitcode2=new int;
    pthread_create(&id1, NULL, func1, (void*)(&flag1));
    pthread_create(&id2, NULL, func2, (void*)(&flag2));
    std::cout<<"\npress any key to stop\n";
    getchar();
    std::cout<<"\nkey is pressed\n";
    flag1 = 0;
    flag2 = 0;
    pthread_join(id1, (void**)exitcode1);
    std::cout<<"\nExitcode1:"<<*exitcode1<<"\n";
    pthread_join(id2, (void**)exitcode2);
    std::cout<<"\nExitcode2:"<<*exitcode2<<"\n";
    delete exitcode1, exitcode2;
    pthread_mutex_destroy(&mut);
    std::cout<<"\nprogram finished\n";
}

