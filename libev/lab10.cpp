#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <ev.h>
#include <stdio.h>
 
static void child_cb (EV_P_ ev_child *watcher, int revents){
	ev_child_stop(EV_A_ watcher);
	int exitcode=WEXITSTATUS(watcher->rstatus);
	std::cout<<"\nchild process finished with "<<exitcode<<"\n";
	ev_break(EV_A_  EVBREAK_ALL);
}

 
int main() {
	std::cout<<"parent process started\n";
	std::cout << "\nMy id : " << getpid() << "\n";
    std::cout << "\nParent id : " << getppid() << "\n";
	char* arg_ptr[]={(char*)"lab3_1.cpp",(char*)"Child", (char*)"process",(char*)"is",(char*)"printing",(char*)"the arguments\n",NULL};
	pid_t pid = fork();
	ev_child child_watcher;
	struct ev_loop *loop = EV_DEFAULT;
    ev_child_init (&child_watcher, child_cb,pid, 0);
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
		ev_child_start(loop, &child_watcher);
		ev_run (loop, 0);

	}
    std::cout<<"\nparent process finished\n";
    return 0;
}
