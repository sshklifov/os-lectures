#include <unistd.h>
#include <sys/wait.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void childFunction(const char* input) {
	execl("/bin/bash", "bash", "-c", input, (char*)NULL);
	exit(127);
}

void parentFunction(pid_t childId) {
	int status;
	waitpid(childId, &status, 0);
	if (WIFEXITED(status)) {
		int childExit = WEXITSTATUS(status);
		if (childExit != 0) {
			printf("my child finished with code %d\n", childExit);
			exit(1);
		}
	}
}

int main() {
	while (1) {
		char prog[4096];
		printf("Type anything you want: ");
		fflush(stdout);
		prog[0] = '\0';
		scanf("%[^\n]%*c", prog);
		if (strlen(prog) == 0) break;

		pid_t childId = fork();
		switch (childId) {
		case -1:
			printf("failed to execute fork\n");
			exit(1);
		case 0:
			childFunction(prog);

		default:
			parentFunction(childId);
		}
	}

	return 0;
}
