#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCK_PATH "echo_socket"

struct cal{
	int a;
	int b;
	char op;
	int result;
};

int main(int argc, char *argv[]){
	int s, n, len;
	struct sockaddr_un remote;
	struct cal cal;

	s=socket(AF_UNIX, SOCK_STREAM, 0);
	printf("연결 중...\n");
	remote.sun_family = AF_UNIX;
	strcpy(remote.sun_path, SOCK_PATH);
	len = strlen(remote.sun_path) + sizeof(remote.sun_family);
	connect(s, (struct sockaddr *)&remote, len);
	printf("연결 성공\n");

	while(1){
		printf("> ");
		scanf("%d %c %d", &cal.a, &cal.op, &cal.b);
		if(send(s,&cal, sizeof(cal), 0 ) == -1) break;
		
		if((n=recv(s,&cal,sizeof(cal),0))>0){
			printf("echo> %d\n", cal.result);
		}else break;
	}
	close(s);

	return 0;

}
