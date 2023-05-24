#include "shell.h"

/**
 * execute_command - Tokenizes user input and executes the command...100%
 * @cmd: Pointer to user input...100%
 * @path: Pointer to PATH environment variable...100%
 * Return void
 */
void execute_command(char *cmd, char *path)
{
	char *token = strtok(cmd, " ");
	char *args[2];
	char *full_path;

	if (token)
	{
		args[0] = strdup(token);
		args[1] = NULL;

		full_path = malloc(sizeof(char) * (strlen(path) + strlen(token) + 2));
		sprintf(full_path, "%s/%s", path, token);

		if (access(full_path, X_OK) == 0)
			execv(full_path, args);
		else
			printf("%s: ERROR: Command not found!\n", token);

		free(full_path);
		free(args[0]);
	}
}

/**
 * print_env - Prints environment variables...100%
 * Return void
 */
void print_env(void)
{
	system("env");
}

/**
 * main - main function of the program...100%
 * Reads user input, parses it & executes the corresp. command...100%
 * Return: 0 on success, non-zero value on failure...100%
 */
int main(void)
{
	char *path = getenv("PATH");
	char *cmd;

	while (1)
	{
		printf(D_PROMPT);

		cmd = malloc(sizeof(char) * 64);
		if (fgets(cmd, 64, stdin) == NULL)
			exit(EXIT_FAILURE);
		cmd[strcspn(cmd, "\n")] = '\0';

		if (strcmp(cmd, "exit") == 0)
		{
			free(cmd);
			exit(EXIT_SUCCESS);
		}

		else if (strcmp(cmd, "env") == 0)
			print_env();
		else
			execute_command(cmd, path);

		free(cmd);
	}
}
