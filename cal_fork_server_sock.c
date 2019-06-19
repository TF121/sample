#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <sys/wait.h>

#define SOCK_PATH "echo_socket"

struct cal{
	int a;
	int b;
	char op;
	int result;
};

int main(int argc, char *argv[]) {
	int s, s2, len;
	unsigned t;
	struct sockaddr_un local, remote;
	struct cal cal;
	pid_t pid;
	int status;

	s = socket(AF_UNIX, SOCK_STREAM, 0);
	local.sun_family = AF_UNIX;
	strcpy(local.sun_path, SOCK_PATH);
	unlink(local.sun_path);
	len = strlen(local.sun_path) + sizeof(local.sun_family);
	bind(s, (struct sockaddr *)&local, len);
	listen(s, 5);

	while(1){
		printf("연결을 기다리는 중...\n");
		int n;
		t = sizeof(remote);
		s2 = accept(s, (struct sockaddr *)&remote, &t);
		printf("연결되었습니다.\n");
		while(1){
			n = recv(s2, &cal, sizeof(cal), 0);
			if(n>0){
				pid = vfork();
				if(pid>0){
					//wait(NULL);
					printf("계산 결과 : %d 전송\n",cal.result);
					if(send(s2,&cal,sizeof(cal),0)<0) break;
				}else if(pid==0){	
					switch(cal.op){
						case '+':
							cal.result = cal.a + cal.b;
							break;
						case '-':
							cal.result = cal.a - cal.b;
							break;
						case '*':
							cal.result = cal.a * cal.b;
							break;
						case '/':
							cal.result = cal.a / cal.b;
							break;
					}
					printf("자식 프로세스에서 %d %c %d 계산\n",cal.a,cal.op,cal.b);
					_exit(0); //to avoid clean shutdown of IO, as the parent might still want to use it
					//return 0;
				}
			}else break;
		}

		close(s2);
	}
	return 0;

}
