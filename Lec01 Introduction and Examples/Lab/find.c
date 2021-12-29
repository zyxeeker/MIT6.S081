#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char* fmtname(char* path){
  	char *p;
  	// Find first character after last slash.
  	for(p=path+strlen(path); p >= path && *p != '/'; p--);
  	p++;
	return p;
}

void find(const char* path,const char* file_name){
	char buf[512], *p;
	int fd;
	struct dirent de;
	struct stat st;

	fd = open(path,0);
	fstat(fd,&st);

	if(strlen(path) + 1 + DIRSIZ +1 > sizeof(buf)){
		printf("find: path too long\n");
		return;
	}
	strcpy(buf,path);
	p = buf+strlen(buf);
	*p++ = '/';
	while(read(fd,&de,sizeof(de)) == sizeof(de)){
		if(de.inum == 0)
			continue;
		memmove(p,de.name,DIRSIZ);
		p[DIRSIZ] = 0;
		if(stat(buf,&st)<0){
			printf("find: cannot stat %s\n",buf);
			continue;
		}

		if(strcmp(fmtname(buf),".") && strcmp(fmtname(buf),"..") && st.type == 1){
			find(buf,file_name);
		}
		if(!strcmp(fmtname(buf),file_name) && st.type == 2){
			printf("%s\n",buf);
		}
	}
	close(fd);
}

int main(int argc,char *argv[]){
	if(argc<2){
		exit(0);
	}
	find(".",argv[1]);
	exit(0);
}
