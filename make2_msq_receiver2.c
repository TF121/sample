#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

typedef struct
{
	long data_type;
	int data_num;
	char data_buff[1024];
} msg_t;

int main(int argc, char *argv[])
{
	int msqid;
	int i;
	msg_t data;
	struct msqid_ds msqstat;


	if(argc<2)
	{
		perror("parameter for the msg type is required");
		exit(1);
	}

	if(-1==(msqid=msgget((key_t)99999,IPC_CREAT|0666)))
	{
		perror("msgget() error");
		exit(1);
	}

	if(-1==msgctl(msqid, IPC_STAT, &msqstat))
	{
		perror("msgctl() error");
		exit(1);
	}
	printf("# of msg in queue: %ld\n", msqstat.msg_qnum);
	
	while(1)
	{
		if(-1==msgrcv(msqid,&data, sizeof(msg_t)-sizeof(long), atoi(argv[1]), 0))
		{
			perror("msgrcv() error");
			exit(1);
		}	
	printf("%d-%s\n", data.data_num, data.data_buff);
	}

	if(-1==msgctl(msqid, IPC_STAT, &msqstat))
	{
		perror("msgctl() error");
		exit(1);
	}
	printf("# of msg in queue: %ld\n", msqstat.msg_qnum);	
	return 0;
}
