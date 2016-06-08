
#include "bank.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

static int msgid1;
static int msgid2;

void init()
{
    msgid1 = msgget(key1,0666|IPC_CREAT|IPC_EXCL);
	 if(msgid1 == -1)
	 {
	     perror("Create Message_Queue1 Failed.");
		  exit(-1);
	 }
	 msgid2 = msgget(key2,0666|IPC_CREAT|IPC_EXCL);
	 if(msgid2 == -1)
	 {
	    perror("Create Message_Queue2 Failed.");
		exit(-1);
	 }
}

void start()
{
	printf("Serever is starting...\n");
	sleep(2);
   // create a child process
	pid_t open_pid = vfork();
	if(open_pid == -1)
	{
		perror("vfork failed");
		exit(-1);
	}
	else if(open_pid == 0)
	{
      execl("open","open",0);
	}
	else
	{
		printf("Waiting for client's choice:\n");
      waitpid(open_pid,0,0);
	}
	printf("Server is now working...\n");
}

void destroy()
{
	printf("Server is shutting down...\n");
	sleep(2);
   if(msgctl(msgid1,IPC_RMID,NULL) == -1)
	{
		perror("Delete Message_Queue1 Failed.");
		exit(-1);
	}
	if(msgctl(msgid2,IPC_RMID,NULL) == -1)
	{
		perror("Delete Message_Queue2 Failed.");
		exit(-1);
	}
	printf("Server has been shut down...\n");
}

void sig_exit()
{
	exit(0);
}

int main()
{
   atexit(destroy);

	printf("To exit, press CTRL+C\n");
	signal(SIGINT,sig_exit);

	//create message queue
	init();

	//start server
	start();
	return 0;
}
