#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h> // For fork, getpid, getppid
#include <sys/wait.h> // For wait
#include <errno.h>
#define EXIT_CANNOT_INVOKE 126   
#define EXIT_ENOENT 127
int main(int argc, char* argv[]){
	bool ignoring_input = isatty(fileno(stdin));
	bool redirecting_stdout = isatty(fileno(stdout));
	bool redirecting_stderr = isatty(fileno(stderr));

	if (ignoring_input)
		freopen("/dev/null", "r", stdin);      

	if (redirecting_stdout){
		if (!freopen("nohup.log", "a", stdout)){
			char *homepath=getenv("HOME");
			char *fullpath=strcat(homepath, "nohup.log");
			if (!freopen(fullpath, "a", stdout))
				exit(EXIT_FAILURE);
		}
	}
	// 將 stderr 合併到 stdout
	if (redirecting_stderr)
		dup2(STDOUT_FILENO, STDERR_FILENO);

	{
	int exit_status;

	signal(SIGHUP, SIG_IGN);
	execvp(argv[1], &argv[1]);
	exit_status = (errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE);

	//exec failed! use exit()
	exit(exit_status);
	}
}
