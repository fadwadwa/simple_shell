#include "main.h"

/**
 * main - Entry point of the shell program
 * @ac: The number of command-line arguments
 * @av: Array of command-line arguments
 * Return: Always 0
 */
int main(int ac, char **av)
{
	char *command = NULL, **tokens = NULL;
	size_t command_len = 0;
	ssize_t nget;
	pid_t pid;

	(void)ac, (void)av;
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
		if (handle_exit_command(command) || handle_env_command(command))
			continue;
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		else if (pid == 0)
		{

			tokens = tokenize_command(command);
			execute_command(tokens);
			free_tokens(tokens);
			exit(EXIT_SUCCESS);
		}
		else
			waitpid(pid, NULL, 0);
	}
	free(command);
	return (0);
}
