#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define SOCK_PATH "msg_socket"

typedef struct user
{	
	char ID[20];
	char PW[20];
	int key;
}user;

typedef struct
{
	long data_type;
	int data_num;
	char data_buff[1024];
} msg_t;

int main(void)
{
	int s, c, len;
	unsigned t;
	struct sockaddr_un local,remote;
	char str[100];
	struct user user;

	int msgkey;
	int msqid;
	int ndx=0;
	msg_t data;

	pid_t pid;

	if ((s=socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
	{
		perror("socket");
		exit(1);
	}

	local.sun_family=AF_UNIX;
	strcpy(local.sun_path,SOCK_PATH);
	unlink(local.sun_path);
	len=strlen(local.sun_path)+sizeof(local.sun_family);

	if(bind(s, (struct sockaddr *)&local,len) == -1)
	{
		perror("bind");
		exit(1);
	}

	if(listen(s,5)== -1)
	{
		perror("listen");
		exit(1);
	}

		
	
	for(;;)
	{
		int done, n;
		printf("연결을 기다리는 중 입니다...\n");
		t=sizeof(remote);

		if((c=accept(s,(struct sockaddr *)&remote,&t)) ==-1)
		{
			perror("accept");
			exit(1);
		}

		printf("연결 성공.\n");

		done =0;

		pid=fork();

		if(pid==0)
		{

			{
				n=recv(c,&user,100,0);
				if (n<=0)
				{
					if (n<0)
						perror("recv");
					done=1;
				}

				user.key=1;
				msgkey=ftok(user.ID, user.key);
if(-1==(msqid=msgget((key_t)msgkey,IPC_CREAT|0666)))
	{
		perror("msgget() error");
		exit(1);
	}
				if (!done)
					if(send(c,&user,n,0)<0)
					{
						perror("send");
						done=1;
					}
			}




			while(1)
			{
				data.data_type=1;
				data.data_num=ndx++;

				sprintf(data.data_buff, "type=%ld, ndx=%d", data.data_type, ndx);
				if(-1==msgsnd(msqid, &data, sizeof(msg_t)-sizeof(long),0))
				{
					perror("msgsnd() error");
					exit(1);
				}
				sleep(1);
				
				if(send(c,&user,n,0)<0)
				{
					perror("send");
					done=1;
				}
			}

			exit(1);
		}
		close(c);
	}


	return 0;
}
