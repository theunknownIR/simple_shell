#include "shell.h"

#define D_PROMPT "$ "
#define MAX_CMD_LENGTH 64

void execute_command(char *cmd, char *path);
void print_env(void);

/**
 * main - main function of the program
 *
 * Reads user input, parses it, & executes the corresponding command
 * Allocate memory for user input...100%
 * Read user input...100%
 * Remove newline character from input...100%
 * Check if user wants to exit the shell...100%
 * Check if user wants to print environment variables...100%
 * Tokenize user input and execute command...100%
 * Search for command in PATH environment variable...100%
 * @argc: argument count number of command line arguments
 * @argv: argument vector
 * Return: 0 on success, non-zero value on failure...100%
 */
int main(int argc, char **argv)
{
	char *cmd;
	char *path = getenv("PATH");

	(void)argc;
	(void)argv;

	while (1)
	{
		printf(D_PROMPT);

		cmd = malloc(sizeof(char) * MAX_CMD_LENGTH);
		if (fgets(cmd, MAX_CMD_LENGTH, stdin) == NULL)
		{
			free(cmd);
			exit(EXIT_FAILURE);
		}
		cmd[strcspn(cmd, "\n")] = '\0';

		if (strcmp(cmd, "") == 0)
		{
			free(cmd);
			continue;
		}
		else if (strcmp(cmd, "exit") == 0)
		{
			free(cmd);
			exit(EXIT_SUCCESS);
		}
		else if (strcmp(cmd, "env") == 0)
		{
			print_env();
		}
		else
		{
			execute_command(cmd, path);
		}

		free(cmd);
	}

	return (0);
}

/**
 * execute_command - Tokenizes user input and executes the command
 * @cmd: Pointer to the user input
 * @path: Pointer to the PATH environment variable
 * Return void
 */
void execute_command(char *cmd, char *path)
{
	pid_t pid;
	char *token;
	char **args;
	char *full_path;
	int i = 0;
	int j;

	token = strtok(cmd, " ");
	args = malloc(sizeof(char *) * (MAX_CMD_LENGTH / 2));

	while (token)
	{
		args[i] = strdup(token);
		token = strtok(NULL, " ");
		i++;
	}
	args[i] = NULL;

	if (i == 0)
	{
		free(args);
		return;
	}

	if (strcmp(args[0], "ls") == 0)
	{
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		else if (pid == 0)
		{
			execvp("ls", args);
			perror("execvp");
			exit(EXIT_FAILURE);
		}
		else
		{
			int status;

			waitpid(pid, &status, 0);
		}
	}
	else
	{
		full_path = malloc(sizeof(char) * (strlen(path) + strlen(args[0]) + 2));
		sprintf(full_path, "%s/%s", path, args[0]);

		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		else if (pid == 0)
		{
			execv(full_path, args);
			perror("execv");
			exit(EXIT_FAILURE);
		}
		else
		{
			int status;

			waitpid(pid, &status, 0);
		}

		free(full_path);
	}

	for (j = 0; j < i; j++)
	{
		free(args[j]);
	}
	free(args);
}

/**
 * print_env - Prints the environment variables
 * Print an error message if execl fails
 * Return void
 */
void print_env(void)
{
	pid_t pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		execl("/usr/bin/env", "env", NULL);
		perror("execl");
		exit(EXIT_FAILURE);
	}
	else
	{
		int status;

		waitpid(pid, &status, 0);
	}
}
