#include <iostream>
#include <unistd.h>
 
int main(int argc, char *argv[]) {
	std::cout<<"\nchild process started\n";
	std::cout << "\nMy id : " << getpid() << "\n";
    std::cout << "\nParent id : " << getppid() << "\n";
    for (int i = 1; i < argc; i++){
		std::cout<<std::flush<<"\e[1;31m"<<argv[i]<<"\e[0m"<<" ";
		sleep(1);
	}
	exit(0);
}
