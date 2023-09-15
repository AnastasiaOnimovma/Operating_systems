#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h> 
#include <unistd.h>
#include <fstream>
#include <iomanip> 

sem_t *sem;
std::ofstream fout;

static void* printing(void* flag){
	int* lc_flag=(int*)flag;
    std::cout<<"\nprinting 2 started\n";
    while(*lc_flag){
    	sem_wait(sem);
		for (int i=0; i<10 && *lc_flag; i++){
			fout<<std::flush;
			fout<<'2';
			std::cout<<std::flush;
			std::cout<<'2';
			sleep(1);
		}
		sem_post(sem);
		sleep(1);
    }
    pthread_exit((void**)2);
}

int main(){
	std::cout<<"program started\n";
	pthread_t id1;
	int flag=1;
	int *exitcode=new int;
	sem = sem_open("/shared_sem", O_CREAT, 0644,1);
	fout.open("lab4.txt",std::ios_base::app);
    pthread_create(&id1, NULL, printing, (void*)(&flag));
	std::cout<<"\npress any key to stop\n";
    getchar();
    std::cout<<"\nkey is pressed\n";
    flag = 0;
	pthread_join(id1, (void**)exitcode);
	sem_close(sem);
	sem_unlink("/shared_sem");
	fout.close();
    std::cout<<"\nprinting 2 finished with:"<<*exitcode<<"\n";
	std::cout<<"\nprogram finished\n";
	return 0;
}
