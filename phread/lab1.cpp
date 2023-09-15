#include <pthread.h>
#include <iostream>
#include <unistd.h>

static void* func1(void* flag){
    int* lc_flag=(int*)flag;
    std::cout<<"\nthread 1 started working\n";
    while(*lc_flag){
    std::cout<<std::flush;
    std::cout<<"1";
    sleep(1);
    }
    std::cout<<"\nthread 1 finished working\n";
    pthread_exit((void**)1);
    
}
static void* func2(void* flag){
    int* lc_flag=(int*)flag;
    std::cout<<"\nthread 2 started working\n";
    while(*lc_flag){
    std::cout<<std::flush;
    std::cout<<"2";
    sleep(1);
    }
    std::cout<<"\nthread 2 finished working";
    pthread_exit((void**)2);
    
}

int main() {
    std::cout<<"program started\n";
    pthread_t id1, id2;
    int flag1=1, flag2=1;
    int *exitcode1=new int, *exitcode2=new int;
    pthread_create(&id1, NULL, func1, (void*)(&flag1));
    pthread_create(&id2, NULL, func2, (void*)(&flag2));
    std::cout<<"press any key to stop\n";
    getchar();
    std::cout<<"\nkey is pressed\n";
    flag1 = 0;
    flag2 = 0;
    pthread_join(id1, (void**)exitcode1);
    std::cout<<"\nExitcode1:"<<*exitcode1<<"\n";
    pthread_join(id2, (void**)exitcode2);
    std::cout<<"\nExitcode2:"<<*exitcode2<<"\n";
    delete exitcode1, exitcode2;
    std::cout<<"\nprogram finished\n";
}

