#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
 
int main() {
	std::cout<<"parent process started\n";
	std::cout << "\nMy id : " << getpid() << "\n";
    std::cout << "\nParent id : " << getppid() << "\n";
    int status;
	char* arg_ptr[]={(char*)"lab3_1.cpp",(char*)"Child", (char*)"process",(char*)"is",(char*)"printing",(char*)"the arguments\n",NULL};
	pid_t pid = fork(),w;
	if(pid==-1){
		perror("fork");
		exit(1);
	}
	else if (pid == 0) {
		int rv= execv("./lab3_1", arg_ptr);
		if(rv == -1){
			perror("execv");
		}
	}	
	else{
		while ((w = waitpid(pid,&status,WNOHANG)) == 0) {
			std::cout<<"\nwaiting\n";
			sleep(2);
		}
		if(w == -1){
			perror("waitpid");
		}
		else{
			std::cout<<"\nchild process finished with "<<status<<"\n";
		}
	}
    std::cout<<"\nparent process finished\n";
    return 0;
}

