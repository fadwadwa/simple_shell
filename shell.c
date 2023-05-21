#include "main.h"

/**
 * main - simple shell
 * Return: always 0 (success)
 */
int main(void)
{
	char *command = NULL;
	size_t command_len = 0;
	ssize_t nget;
	pid_t pid;
	char *token, **tokens = NULL;
	int num_tokens = 0;

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
			token = strtok(command, " \t\n");
			while (token != NULL)
			{
				tokens = realloc(tokens, (num_tokens + 1) * sizeof(char *));
				if (tokens == NULL)
				{
					perror("realloc");
					exit(EXIT_FAILURE);
				}
				tokens[num_tokens] = strdup(token);
				num_tokens++;
				token = strtok(NULL, " \t\n");
			}
			tokens = realloc(tokens, (num_tokens + 1) * sizeof(char *));
			tokens[num_tokens] = NULL;
			free(token);
			if (tokens[0][0] == '/')
			{
				if (execve(tokens[0], tokens, NULL) == -1)
				{
					perror("execve");
					exit(EXIT_FAILURE);
				}
			}
			else
			{
				if (execve(tokens[0], tokens, NULL) == -1)
				{
					perror("execve");
					free(command);
					exit(EXIT_FAILURE);
				}
			}
		}
		else
			waitpid(pid, NULL, 0);
	}
	free(command);
	return (0);
}
