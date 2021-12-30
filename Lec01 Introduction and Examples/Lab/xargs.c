#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

int main(int argc,char *argv[]){
	if(argc < 2)
		exit(0);
	char buf[MAXARG];
	char *args[MAXARG];
	int n,i;
	if((n = read(0,&buf,sizeof(buf)))<0)
		exit(0);
	for(i = 1;i<argc;++i){
		args[i-1] = argv[i];
	}
	int t = 0;
	while(t<n){
		if(buf[t] == '\n'){
			buf[t] = '\0';
			char tmp[1];
			args[i-1] = strcpy(tmp,buf);
			args[i] = 0;
		if(fork()==0){
			exec(argv[1],args);
		}else
			wait(0);
		}
		++t;
	}
	exit(0);
}
