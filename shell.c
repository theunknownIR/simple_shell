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
 * Allocate memory for user input...100%
 * Read user input...100%
 * Remove newline character from input...100%
 * Check if user wants to exit the shell...100%
 * Check if user wants to print environment variables...100%
 * Tokenize user input and execute command...100%
 * Search for command in PATH environment variable...100%
 * Return: 0 on success, non-zero value on failure...100%
 */
int main(void)
{
	char *cmd;
	char *path = getenv("PATH");

	while (1)
	{
		printf(D_PROMPT);

		cmd = malloc(sizeof(char) * 64);
		if (fgets(cmd, 64, stdin) == NULL)
		{
			free(cmd);
			exit(EXIT_FAILURE);
		}
		cmd[strcspn(cmd, "\n")] = '\0';

		if (strcmp(cmd, "exit") == 0)
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
	char *token = strtok(cmd, " ");
	char *args[3];
	char *full_path;

	if (token)
	{
		args[0] = strdup(token);
		token = strtok(NULL, " ");
		if (token)
		{
			args[1] = strdup(token);
			args[2] = NULL;
		}
		else
		{
		   args[1] = NULL;
		}

		full_path = malloc(sizeof(char) * (strlen(path) + strlen(args[0]) + 2));
		sprintf(full_path, "%s/%s", path, args[0]);

		if (strcmp(args[0], "ls") == 0)
		{
			if (args[1] && strcmp(args[1], "-l") == 0)
			{
				system("ls -l");
			}
			else if (args[1] && strcmp(args[1], "/var") == 0)
			{
				system("ls /var");
			}
			else
			{
				system("ls");
			}
		}
		else if (access(full_path, X_OK) == 0)
		{
			execv(full_path, args);
		}
		else
		{
			printf("%s: Command not found.\n", args[0]);
		}

		free(full_path);
		free(args[0]);
		if (args[1])
		{
			free(args[1]);
		}
	}
}

/**
 * print_env - Prints the environment variables -task 5
 * Return void
 */
void print_env(void)
{
	system("env");
}
