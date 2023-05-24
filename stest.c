#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define D_PROMPT "$ "

/**
 * execute_command - Tokenizes user input and executes the command
 * print_env - Prints the environment variables
 * @cmd: Pointer to the user input
 * @path: Pointer to the PATH environment variable
 * Return void
 */
void execute_command(char *cmd, char *path);
void print_env(void);

/**
 * main - main function of the program
 *
 * Reads user input, parses it, & executes the corresp. command
 * Allocate memory for user input
 * Read user input
 * Remove newline character from input
 * Check if user wants to exit the shell
 * Check if user wants to print environment variables
 * Tokenize user input and execute command
 * Search for command in PATH environment variable
 * Return: 0 on success, non-zero value on failure
 */
int main(void)
{
	char *cmd;
	char *path = getenv("PATH");

	while (1)
	{
		printf(D_PROMPT);

		cmd = malloc(sizeof(char) * 64);
		if (cmd == NULL)
		{
			perror("malloc");
			exit(EXIT_FAILURE);
		}

		if (fgets(cmd, 64, stdin) == NULL)
		{
			perror("fgets");
			exit(EXIT_FAILURE);
		}

		cmd[strcspn(cmd, "\n")] = '\0';

		if (strcmp(cmd, "exit") == 0)
		{
			free(cmd);
			exit(EXIT_SUCCESS);
		}

		if (strcmp(cmd, "env") == 0)
		{
			print_env();
			continue;
		}

		execute_command(cmd, path);

		free(cmd);
	}

	return (0);
}

/**
 * execute_command - Tokenizes user input and executes the command
 * Return void
 */
void execute_command(char *cmd, char *path)
{
	char *token = strtok(cmd, " ");

	if (token)
	{
		char *args[] = {token, NULL};
		char *full_path = malloc(sizeof(char) * (strlen(path) + strlen(token) + 2));

		if (full_path == NULL)
		{
			perror("malloc");
			exit(EXIT_FAILURE);
		}
		while (token != NULL)
			token = strtok(NULL, " ");
		sprintf(full_path, "%s/%s", path, token);
		if (access(full_path, X_OK) == 0)
		{
			execv(full_path, args);
		}
		else
		{
			printf("%s: Command not found.\n", token);
		}
		free(full_path);
	}
}

/**
 * print_env - Prints the environment variables
 * Return void
 */
void print_env(void)
{
	system("env");
}
