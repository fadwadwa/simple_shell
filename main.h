#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
extern char **environ;

/* EXECUTION */
char *find_executable_path(char *command);
void execute_command(char **tokens);

/* COMMANDS */
int handle_exit_command(char *command);
int handle_env_command(char *command);

/* TOKENIZER */
char **tokenize_command(char *command);
void free_tokens(char **tokens);
#endif
