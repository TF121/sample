#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCK_PATH "cal_socket"

typedef struct massage
{
	int type;
	int operator;
	double value[2];
}massage;

int main(void)
{
	int s,t, len;
	struct sockaddr_un remote;
	char str[100];

	struct massage msg;
	msg.type=1;

	if ((s=socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
	{
		perror("socket");
		exit(1);
	}

	printf("연결을 시도하는 중입니다...\n");

	remote.sun_family=AF_UNIX;
	strcpy(remote.sun_path, SOCK_PATH);
	len =strlen(remote.sun_path)+sizeof(remote.sun_family);

	if (connect(s, (struct sockaddr *)&remote, len) == -1)
	{
		perror("connect");
		exit(1);
	}

	printf("연결 성공.\n");

	while(1)
	{
		msg.type=1;
		printf("첫번째 숫자 입력... \n");
		scanf("%lf",&msg.value[0]);
		printf("연산자 입력... \n1 : +, 2 : -, 3 : *, 4 : /\n");
		scanf("%d",&msg.operator);
		printf("두번째 숫자 입력...\n");
		scanf("%lf",&msg.value[1]);


		if (send(s,&msg,sizeof(msg),0) == -1)
		{
			perror("send");
			exit(1);
		}

		if ((t=recv(s,&msg,100,0))>0)
		{
			if(msg.type==2)
			{
				printf("결과 : %.2f\n", msg.value[0]);
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

