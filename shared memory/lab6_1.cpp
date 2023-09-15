#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h> 
#include <sys/mman.h>
#include <unistd.h>
#include <fstream>
#include <iomanip> 
#include <cstring>
#include <signal.h>

sem_t *sem_read,*sem_write;
char* loc_var;
off_t  length = 50;
int fd;

void sig_handler(int signo){
	std::cout<<"\nFree up the memory\n";
	sem_close(sem_read);
	sem_close(sem_write);
	sem_unlink("/read_sem");
	sem_unlink("/write_sem");
	munmap(loc_var, length);
	close(fd);
	shm_unlink("/shared_memory");
	exit(0);
}

static void* reading(void* flag){
	std::cout<<"\nreading started\n";
	int* lc_flag=(int*)flag;
	char buf[length];
    while(*lc_flag){
    	sem_wait(sem_write);
		memcpy(buf,loc_var,length);
		std::cout<<buf<<"\n";
		sem_post(sem_read);
    }
    pthread_exit((void**)1);
}

int main(){
	std::cout<<"program started\n";
	pthread_t id1;
	int flag=1;
	int *exitcode=new int;
	fd = shm_open("/shared_memory", O_CREAT|O_RDWR, 0644 );
	ftruncate(fd, length+1);
	loc_var = (char*)mmap(NULL, length+1, PROT_WRITE|PROT_WRITE, MAP_SHARED, fd, 0);
	sem_read = sem_open("/read_sem", O_CREAT, 0644,1);
	sem_write = sem_open("/write_sem", O_CREAT, 0644,1);
    pthread_create(&id1, NULL, reading, (void*)(&flag));
	std::cout<<"\npress any key to stop\n";
	signal(SIGINT,sig_handler);
    getchar();
    std::cout<<"\nkey is pressed\n";
    flag = 0;
	pthread_join(id1, (void**)exitcode);
	sem_close(sem_read);
	sem_close(sem_write);
	sem_unlink("/read_sem");
	sem_unlink("/write_sem");
	munmap(loc_var, length);
	close(fd);
	shm_unlink("/shared_memory");
    std::cout<<"\nreading finished with:"<<*exitcode<<"\n";
	std::cout<<"\nprogram finished\n";
	return 0;
}
