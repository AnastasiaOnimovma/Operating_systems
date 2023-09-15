#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/capability.h>

int main() {
	std::cout<<"\nprogram started\n";
	cap_t cap_set,cap_get; 
	ssize_t *length_p;
	const char* file_path="lab9_1";
	const char* caps="cap_chown=+eip";
	char *buf;
	cap_get = cap_get_file(file_path);
	if(cap_get==NULL){
		perror("cap_get_file");
	}
	else{
		buf=cap_to_text(cap_get,length_p);
		std::cout<<"Capabitities of "<<file_path<<": "<<buf<<"\n";
		cap_free(buf);
	}
	file_path="lab9_2";
	cap_set = cap_from_text(caps);
	if(cap_set==NULL){
		perror("cap_from_text");
	}
	int result = cap_set_file(file_path,cap_set);
	if(result==-1){
		perror("cap_set_file");
	}
	else{
		std::cout<<"Successfully set the capabilities for "<<file_path<<": "<<caps;
	}
    std::cout<<"\nprogram finished\n\n";
    return 0;
}
