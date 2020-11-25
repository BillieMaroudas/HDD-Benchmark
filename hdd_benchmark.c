#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>

ssize_t bytes=0;
ssize_t total=0; 

time_t start;
time_t finish;

extern int errno;

int fd;
char NAME[255];

#define FILENAME argv[1]?argv[1]:"benchmark.txt"

/* function that stops a process*/
void Exit()
{
 close(fd);	/*close the file descriptor*/
 unlink(NAME);	/*remove the link to the file*/
 fflush(stdout);	/*flush the buffer*/
 exit(0);	/* terminate the process normally*/
}

/*function for alarm set up*/
void Alarm()
{ 
 printf("\rSize:%03.3f Mb Speed:%03.3f Mbytes/sec ", (float)total/(1024*1024),(float)bytes/(1024*1024));
 bytes=0;
 fflush(stdout);	/*flush the buffer*/  
 signal(SIGALRM,(void *)Alarm);		/*install alarm handler*/
 alarm(1);	/*schedule alarm signal for every second*/
 
}

#define BUF_SIZE (1024*7)
#define FILE_SIZE (float)4*1024*1024*1024

/*function testing writing speed*/
int write_benchmark(int argc, char * argv[])
{
 char buf[BUF_SIZE];
 ssize_t s;
 size_t size=BUF_SIZE;
 
 sprintf(NAME,"%s",FILENAME);
 
 printf("Writing to %s \n",NAME);
 
 unlink(NAME);

 time(&start);	/*start time in seconds*/
 
 for(;;)	/*infinite loop*/
 	{s=write(fd,buf,size);	/*writes to the given file descriptor and returns the number of bytes that were written*/
          
		if (s<0)
			{printf("An error occured!\n");
			return -1;}
		else if (total >FILE_SIZE) { 
                     time(&finish);	/*finish time in seconds*/
                     printf(" Writing finished \n");
                     printf("Average write speed %03.3f MBytes/sec \n",((float) total)/(1024*1024*difftime(finish, start)));	/*difftime returns the elapsed time in seconds from start to finish time*/
                     return(0);
                   }
		else{	          
         		bytes+=s;
          		total+=s;
		}
         }

 return 0;
}

/*function testing reading speed*/
int read_benchmark(int argc, char * argv[])
{
 char buf[BUF_SIZE];
 ssize_t s;
 
 printf("Reading from %s \n",NAME);
 
 s=lseek(fd,0,SEEK_SET);	/*find the start of the file*/
 
 total=0;
 bytes=0;

 time(&start);		/*start time in seconds*/
  
 if (s!=0) {printf("Cannot read file %s %s \n",NAME,sys_errlist[errno]);}
 
 for(;;) 	/*infinite loop*/
{s=read(fd,buf,BUF_SIZE);	/*reads from the given file descriptor and returns the number of bytes that were read*/
         	if (s<=0){close(fd);	/*close the file descriptor*/
                    	time(&finish);	/*finish time in seconds*/
                     	unlink(NAME);	/*remove the link to the file*/
			printf(" Reading finished \n");
                     	printf("Average read speed %03.3f MBytes/sec \n",((float) total)/(1024*1024*difftime(finish,start)));	/*difftime returns the elapsed time in secondsfrom start to finish time*/
                     	printf("Read %ld bytes \n",total); 
                     	return(0);
                   	}
		
                bytes+=s;
          	total+=s;
         	
	}

 return 0;
}


int main(int argc, char * argv[])
{
 char buf[BUF_SIZE];
 ssize_t s;
 
 sprintf(NAME,"%s",FILENAME);
 
 printf("Open file %s \n",NAME);
 
 unlink(NAME);
 
/*If the file does not exist, create it. Open the file so that it can be read from and written to. Initially clear all data from the file.*/
 fd=open(NAME,O_CREAT | O_RDWR | O_TRUNC /*| O_LARGEFILE*/  );
 
 if (fd<=0) {
        printf("\n Open() failed!");
        return 1;
    };
 
 atexit(Exit);	/*register function Exit to be called for normal process termination*/
 
 signal(SIGQUIT,(void *)Exit);	/*setup quit handler*/
 signal(SIGABRT,(void *)Exit);	/*setup abort handler*/
 signal(SIGINT,(void *)Exit);	/*setup interrupt handler*/
 
 signal(SIGALRM,Alarm);		/*setup alarm handler*/
 
 alarm(1);		/*schedule alarm signal for every second*/

 write_benchmark(argc,argv);
 read_benchmark(argc,argv);
 
 return 0;
}
