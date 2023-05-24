#include "main.h"

/**
 * tokenize_command - Tokenizes a command into an array of tokens
 * @command: The command to tokenize
 * Return: Array of tokens
 */
char **tokenize_command(char *command)
{
	int num_tokens = 0;
	char **tokens = NULL;
	char *token = strtok(command, " \t\n");

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
 * free_tokens - Frees the memory allocated for an array of tokens
 * @tokens: The array of tokens to free
 */
void free_tokens(char **tokens)
{
	if (tokens == NULL)
		return;

	for (int i = 0; tokens[i] != NULL; i++)
	{
		free(tokens[i]);
	}

	free(tokens);
}
