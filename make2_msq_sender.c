#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

typedef struct
{
	long data_type;
	int data_num;
	char data_buff[1024];
} msg_t;

int main()
{
	int msqid1, msqid2;
	int ndx=0;
	msg_t data;
	pid_t pid;

	if(-1==(msqid1=msgget((key_t)9999,IPC_CREAT|0666)))
	{
		perror("msgget() error");
		exit(1);
	}
	
	if(-1==(msqid2=msgget((key_t)99999,IPC_CREAT|0666)))
	{
		perror("msgget() error");
		exit(1);
	}

	pid=fork();

	while(1)
	{
		if(pid==0)
		{
			data.data_type=2;
			data.data_num=ndx++;

			sprintf(data.data_buff, "type=%ld, ndx=%d", data.data_type, ndx);
			if(-1==msgsnd(msqid2, &data, sizeof(msg_t)-sizeof(long),0))
			{
				perror("msgsnd() error");
				exit(1);
			}
		}else
		{
			data.data_type=1;
			data.data_num=ndx++;

			sprintf(data.data_buff, "type=%ld, ndx=%d", data.data_type, ndx);

			if(-1==msgsnd(msqid1, &data, sizeof(msg_t)-sizeof(long),0))
			{
				perror("msgsnd() error");
				exit(1);
			}
		}
		sleep(1);
	}
	return 0;
}
