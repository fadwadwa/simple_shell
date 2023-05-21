#include "main.h"

char *find_path(char *command);
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
	char *cmd_path;

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
		if (strcmp("exit", command) == 0)
			exit(EXIT_SUCCESS);
		if (strcmp("env", command) == 0)
		{
			char **env = environ;
			while (*env != NULL)
			{
				printf("%s\n", *env);
				env++;
			}
			continue;
		}
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

			// Handle the PAtH

			free(token);
			cmd_path = find_path(tokens[0]);
			// check for null

			tokens[0] = cmd_path;

			if (execve(tokens[0], tokens, NULL) == -1)
			{
				perror("execve");
				exit(EXIT_FAILURE);
			}
		}
		else
			waitpid(pid, NULL, 0);
	}
	free(command);
	return (0);
}

char *find_path(char *command)
{
	char *path, *path_copy, *dir_path, *new_path;
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
