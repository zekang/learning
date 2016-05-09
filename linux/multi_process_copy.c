#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<fcntl.h>

int mycopy(int n,const char *src,const char *target,int start,int size)
{
		int fd_read,fd_write;
		int write_size = 0;
		char buf[1024]={0};
		ssize_t readlen = 0;
		fd_read = open(src,O_RDONLY);		
		printf("\nprocess %d,start %d,size %d\n",n,start,size);
		if(fd_read < 0){
				perror("open src");
				return -1;
		}
		if(lseek(fd_read,start,SEEK_SET) == -1){
				perror("lseek src");
				return -1;
		}
		umask(0);
		fd_write = open(target,O_CREAT|O_WRONLY,0644);
		if(fd_write < 0){
				perror("open target");
				return -1;
		}
		if(lseek(fd_write,start,SEEK_SET) == -1){
				perror("lseek target");
				return -1;
		}
		do{
				readlen = read(fd_read,buf,sizeof(buf));	
				if(readlen >0 ){
						if(write_size + readlen > 	size ){
								readlen = size - write_size;
						}
						write(fd_write,buf,readlen);
						write_size += readlen;
				}else{
						break;
				}
		}while(1);
		close(fd_read);
		close(fd_write);
		return 0;
}

int main(int argc,char *argv[])
{
		int filesize = 0 ;
		int copysize = 1024 * 1024 * 2;
		int count ,i;
		struct stat st;
		pid_t pid;
		if(argc < 3){
				printf("useage:cp source target\n");
				return -1;
		}

		if(stat(argv[1],&st) != 0){
				perror("stat");
				return -1;
		}
		count = 	st.st_size  /  copysize  ;
		if(st.st_size % copysize !=0){
				count++;
		}
		printf("total %d,count %d\n",st.st_size,count);
		for(i=0;i<count;i++){
				pid = fork();	
				if(pid == 0){
						mycopy(i,argv[1],argv[2],i * copysize ,copysize);	
						break;
				}
		}	
		if(pid > 0){
				wait(NULL);
		}
		return 0;
}
