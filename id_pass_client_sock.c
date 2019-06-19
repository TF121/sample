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

int main()
{
	int s,t, len;
	struct sockaddr_un remote;

	struct user user;

	int msgkey;

	int msqid;
    int i;
    msg_t data;
    struct msqid_ds msqstat;

	if ((s=socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
	{
		perror("socket");
		exit(1);
	}

	printf("서버와 연결을 시도하는 중입니다...\n");

	remote.sun_family=AF_UNIX;
	strcpy(remote.sun_path, SOCK_PATH);
	len =strlen(remote.sun_path)+sizeof(remote.sun_family);

	if (connect(s, (struct sockaddr *)&remote, len) == -1)
	{
		perror("connect");
		exit(1);
	}

	printf("서버 연결 성공.\n");

	while(1)
	{
		printf("ID : ");
		scanf("%s",user.ID);
		printf("PW : ");
		scanf("%s",user.PW);

		if (send(s,&user,sizeof(user),0) == -1)
		{
			perror("send");
			exit(1);
		}

		if ((t=recv(s,&user,100,0))>0)
		{
			printf("할당받은 메세지 큐에 연결합니다.\n");
			
			sleep(1);

			msgkey=ftok(user.ID, user.key);

			if(-1==(msqid=msgget((key_t)msgkey,IPC_CREAT|0666)))
		    {
		        perror("msgget() error");
		        exit(1);
		    }
		
			while(1)
			{
				if(-1==msgrcv(msqid,&data, sizeof(msg_t)-sizeof(long), 1    , 0))
				{
					perror("msgrcv() error");
					exit(1);
				}
				printf("%s\n",data.data_buff);
			}
		} else
		{
			if(t<0) perror("recv");
			else printf("서버와 연결이 끊겼습니다.\n");
			exit(1);
		}
	}

	close(s);

	return 0;
}

