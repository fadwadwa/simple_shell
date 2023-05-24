#include "main.h"

char *find_path(char *command);

/**
 * read_line - Read a line of input from stdin.
 * Return: the read line.
 */
char *read_line()
{
	char *line = NULL;
	size_t line_len = 0;
	ssize_t nget;

	
	nget = getline(&line, &line_len, stdin);

	if (nget == -1)
	{
		free(line);
		exit(EXIT_SUCCESS);
	}

	if (line[nget - 1] == '\n')
		line[nget - 1] = '\0';

	return (line);
}

/**
 * tokenize_line - Tokenize the input line.
 * @line: ptr to char
 * Return: an array of tokens.
 */
char **tokenize_line(char *line)
{
	char *token, **tokens = NULL;
	int num_tokens = 0;

	token = strtok(line, " \t\n");
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

	return (tokens);
}

/**
 * free_tokens - Free the allocated memory for the tokens.
 * @tokens: double ptr to tokens
 * Return: void
 */
void free_tokens(char **tokens)
{
	char **ptr = tokens;

	while (*ptr != NULL)
	{
		free(*ptr);
		ptr++;
	}

	free(tokens);
}

/**
 * execute_command - Execute the command with the specified tokens.
 * @tokens: double pointer to tokens
 * Return: void
 */
void execute_command(char **tokens)
{
	char *cmd_path;
	pid_t pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{

		cmd_path = find_path(tokens[0]);

		tokens[0] = cmd_path;

		if (execve(tokens[0], tokens, NULL) == -1)
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
/**
 * find_path - Find the path of the command
 * in the system's PATH variable.
 * @command: the command
 * Return: the path if found, otherwise returns NULL.
 */
char *find_path(char *command)
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
 * main - the main fct
 * Return: 0
 */
int main(void)
{
	char *command;
	char **tokens;

	while (1)
	{
		command = read_line();

		if (command[0] == '\0')
		{
			free(command);
			continue;
		}

		if (strcmp("exit", command) == 0)
		{
			free(command);
			exit(EXIT_SUCCESS);
		}

		if (strcmp("env", command) == 0)
		{
			char **env = environ;

			while (*env != NULL)
			{
				printf("%s\n", *env);
				env++;
			}

			free(command);
			continue;
		}

		tokens = tokenize_line(command);
		free(command);

		execute_command(tokens);
		free_tokens(tokens);
	}

	return (0);
}
