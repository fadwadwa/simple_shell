#include "main.h"

/**
 * find_executable_path - Finds the executable path for a command
 * @command: The command to find the executable path for
 * Return: The executable path or NULL if not found
 */
char *find_executable_path(char *command)
{
	char *path, *dir_path, *new_path;
	int dir_len, cmd_len;
	struct stat st;

	path = getenv("PATH");
	if (stat(command, &st) == 0)
		return (command);
	if (path)
	{
		cmd_len = strlen(command);
		dir_path = strtok(path, ":");
		while (dir_path != NULL)
		{
			dir_len = strlen(dir_path);
			new_path = calloc(dir_len + cmd_len + 2, sizeof(char));
			if (!new_path)
			{
				perror("calloc");
				exit(EXIT_FAILURE);
			}
			strcpy(new_path, dir_path);
			strcat(new_path, "/");
			strcat(new_path, command);

			if (stat(new_path, &st) == 0)
				return (new_path);
			free(new_path);
			dir_path = strtok(NULL, ":");
		}
	}
	return (NULL);
}

/**
 * execute_command - Executes a command with the given tokens
 * @tokens: Array of tokens representing the command and its arguments
 */
void execute_command(char **tokens)
{
	char *cmd_path = find_executable_path(tokens[0]);

	if (cmd_path == NULL)
	{
		fprintf(stderr, "Command not found: %s\n", tokens[0]);
		exit(EXIT_FAILURE);
	}

	tokens[0] = cmd_path;

	if (execve(tokens[0], tokens, NULL) == -1)
	{
		perror("execve");
		exit(EXIT_FAILURE);
	}
}
