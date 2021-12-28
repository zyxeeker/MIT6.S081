#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(){
	int fd[2];
	int _fd[2];
	pipe(fd);
	pipe(_fd);

	int pid = fork();
	char buf[10];

	if(pid == 0){
		while(read(fd[0],buf,sizeof(buf))){
			printf("%d: received ping\n",getpid());
			write(_fd[1],"s",1);
		}
		exit(0);
	}else{
		write(fd[1],"s",1);
		while(read(_fd[0],buf,sizeof(buf))){
			printf("%d: received pong\n",getpid());
			exit(0);
		}
	}
	exit(0);
}

