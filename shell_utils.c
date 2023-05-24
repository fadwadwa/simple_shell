#include "main.h"

/**
 * handle_env_command - Handles the "env" command
 * @command: The command to handle
 * Return: 1 if the command is "env", 0 otherwise
 */
int handle_env_command(char *command)
{
	if (strcmp("env", command) == 0)
	{
		char **env = environ;

		while (*env != NULL)
		{
			printf("%s\n", *env);
			env++;
		}
		return (1);
	}
	return (0);
}

/**
 * handle_exit_command - Handles the "exit" command
 * @command: The command to handle
 * Return: 1 if the command is "exit", 0 otherwise
 */
int handle_exit_command(char *command)
{
	if (strcmp("exit", command) == 0)
		exit(EXIT_SUCCESS);

	return (0);
}
