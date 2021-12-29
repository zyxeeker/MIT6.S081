#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void sieve(int *left_pipe){
	int p;
	read(left_pipe[0],&p,sizeof(p));
	if(p == -1)
		exit(0);

	printf("prime %d\n",p);

	int right_pipe[2];
	pipe(right_pipe);

	if(fork()==0){
		close(left_pipe[0]);
		close(right_pipe[1]);
		sieve(right_pipe);
	}else{
		close(right_pipe[0]);
		int buf;
		while(read(left_pipe[0],&buf,sizeof(buf)) && buf != -1){
			if(buf % p != 0)
				write(right_pipe[1],&buf,sizeof(buf));
		}
		write(right_pipe[1],&buf,sizeof(buf));
		close(left_pipe[0]);
		wait(0);
		exit(0);

	}
}

int main(){
	int fd[2];
	pipe(fd);
	if(fork()==0){
		close(fd[1]);
		sieve(fd);
		exit(0);
	}else{
		close(fd[0]);
		for(int i = 2;i<36;++i){
			write(fd[1],&i,sizeof(i));
		}
		int i = -1;
		write(fd[1],&i,sizeof(i));
	}
	wait(0);
	exit(0);
}

