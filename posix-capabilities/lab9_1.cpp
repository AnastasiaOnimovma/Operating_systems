#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>


int main(int argc, char *argv[]) {
	std::cout<<"\nprogram started\n";
	if (argc != 2) {
    	std::cout<<"Wrong number of the arguments\n";
    	exit(0);
    }
    const char *file_path=argv[0],
     *user_name=argv[1];
	uid_t uid;
	gid_t gid;
	passwd *pwd;
	pwd = getpwnam(user_name);
	if (pwd == NULL) {
      perror("getpwnam");
      std::cout<<"Failed to get uid\n";
      exit(0);
    }
    uid = pwd->pw_uid;
    if (chown(file_path, uid, -1) == -1) {
      perror("chown");
  	}
  	else{
  		std::cout<<"User changed: "<<user_name;
   	}
    std::cout<<"\nprogram finished\n";
    return 0;
}
