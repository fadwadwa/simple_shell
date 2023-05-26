#include "main.h"

char *find_path(char *command);
/**
 * space_trim - trim spaces and \t
 * @str: the line
 * Return: void
 */
void space_trim(char *str)
{
	int i = 0, j = 0, len = strlen(str);

	while (str[i] == ' ' || str[i] == '\t')
		i++;

	while (i < len)
		str[j++] = str[i++];

	str[j] = '\0';

	i = strlen(str) - 1;
	while (i > 0 && (str[i] == ' ' || str[i] == '\t'))
		str[i--] = '\0';
}
/**
 * read_line - Read a line of input from stdin.
 * Return: the read line.
 */
char *read_line(void)
{
	char *line = NULL;
	size_t line_len = 0;
	ssize_t nget;

	if (isatty(STDIN_FILENO))
		printf("$ ");
	nget = getline(&line, &line_len, stdin);

	if (nget == -1)
	{
		free(line);
		exit(EXIT_SUCCESS);
	}

	if (line[nget - 1] == '\n')
		line[nget - 1] = '\0';
	space_trim(line);
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
 * @status: a pointer to the status of the last executed command
 * Return: void
 */
void execute_command(char **tokens, int *status)
{
	char *cmd_path;
	pid_t pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		free_tokens(tokens);
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{

		cmd_path = find_path(tokens[0]);
		if (cmd_path == NULL)
		{
			perror("find_path");
			free_tokens(tokens);
			exit(EXIT_FAILURE);
		}
		tokens[0] = cmd_path;

		if (execve(tokens[0], tokens, NULL) == -1)
		{
			perror("execve");
			free_tokens(tokens);
			exit(EXIT_FAILURE);
		}
		free_tokens(tokens);
	}
	else
	{
		waitpid(pid, status, 0);
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

	if (stat(command, &st) == 0)
		return (command);
	path = getenv("PATH");
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
 * remove_comment - a function to remove comments
 * @line: the command line
 */
void remove_comment(char *line)
{
	char *comment;

	comment = strchr(line, '#');
	if (line[0] == '#' || (comment != NULL && *(comment - 1) == ' '))
		*comment = '\0';
}

/**
 * change_directory - Change the current directory.
 * @tokens: The tokens containing the command and directory.
 */
void change_directory(char **tokens)
{
	char buffer[1024];
	char *dir;

	if (tokens[1] == NULL || strcmp(tokens[1], "~") == 0)
		dir = getenv("HOME");
	else if (strcmp(tokens[1], "-") == 0)
		dir = getenv("OLDPWD");
	else
		dir = tokens[1];

	getcwd(buffer, sizeof(buffer));

	if (chdir(dir) != 0)
		perror("cd");

	setenv("PWD", getcwd(buffer, sizeof(buffer)), 1);
}

/**
 * main - the main fct
 * Return: Always 0
 */
int main(void)
{
	char *command;
	char **tokens = NULL;
	int last_status = 0;

	while (1)
	{
		command = read_line();
		remove_comment(command);

		if (command[0] == '\0')
		{
			free(command);
			continue;
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
		if (strcmp("exit", command) == 0)
		{
			if (tokens[1] != NULL && (atoi(tokens[1]) >= 0 && atoi(tokens[1]) <= 255))
				last_status = atoi(tokens[1]);
			free(command);
			free_tokens(tokens);
			exit(last_status);
		}
		free(command);
		if (strcmp(tokens[0], "cd") == 0)
			change_directory(tokens);
		else
			execute_command(tokens, &last_status);
		free_tokens(tokens);
		if (WIFEXITED(last_status))
			last_status = WEXITSTATUS(last_status);
	}

	return (0);
}
