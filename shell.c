#include "main.h"

/**
 * main - simple shell
 * Return: always 0 (success)
 */
int main()
{
	char *command = NULL;
	size_t command_len = 0;
	ssize_t nget;
	pid_t pid;
	char *args[] = {NULL, NULL};

	while (1)
	{
		printf("$ ");
		nget = getline(&command, &command_len, stdin);
		if (nget == -1)
		{
			perror("getline");
			exit(EXIT_FAILURE);
		}

		if (command[nget - 1] == '\n')
			command[nget - 1] = '\0';

		args[0] = command;
		if (command[0] == '\0')
			continue;

		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		else if (pid == 0)
		{
			if (execve(command, args, NULL) == -1)
			{
				perror("execve");
				exit(EXIT_FAILURE);
			}
		}
		else
		{

			waitpid(pid, NULL, 0);
		}
	}
	free(command);
	return (0);
}