#include <unistd.h>
#include <sys/wait.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* cwd = "/home/stef/Downloads/os-lectures/lect3";
void childFunction(const char* name) {
	char exec[4096];
	exec[0] = '\0';
	strcat(exec, cwd);
	strcat(exec, "/");
	strcat(exec, name);
	execl(exec, name, "1", "2", "3", "4", (char*)NULL);
	exit(127);
}

void parentFunction(pid_t childId) {
	printf("this is the parent, my child is %d\n", childId);
	int status;
	waitpid(childId, &status, 0);
	if (WIFEXITED(status)) {
		int childExit = WEXITSTATUS(status);
		printf("my child finished with code %d\n", childExit);
		if (childExit != 0) {
			exit(1);
		}
		else {
			exit(0);
		}
	}
}

int main() {
	pid_t childId = fork();
	switch (childId) {
	case -1:
		printf("failed to execute fork\n");
		exit(1);
	case 0:
		childFunction("mySumProgram");

	default:
		parentFunction(childId);
	}

	return 0;
}
