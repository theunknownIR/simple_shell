#include "shell.h"

/**
 * main - main function of the program
 *
 * La Description: Reads user input, parses it, and executes the corresponding command
 * Allocating memory for user input...100%
 * Reading user input...100%
 * Removing newline character from input...100%
 * Checking if user wants to exit the shell...100%
 * Checking if user wants to print environment variables...100%
 * Tokenizing user input and execute command...100%
 * Searching for command in PATH environment variable...100%
 * 
 * 
 * Return: 0 on success, non-zero value on failure
 */
int main(void)
{
	char *cmd;
	char *token;
	char *path = getenv("PATH");

	while (1) {
		printf(D_PROMPT);

		cmd = malloc(sizeof(char) * 64);
		if (cmd == NULL) {
			perror("malloc");
			exit(EXIT_FAILURE);
		}

		if (fgets(cmd, 64, stdin) == NULL) {
			perror("fgets");
			exit(EXIT_FAILURE);
		}

		cmd[strcspn(cmd, "\n")] = '\0';

		if (strcmp(cmd, "exit") == 0) {
			free(cmd);
			exit(EXIT_SUCCESS);
		}

		if (strcmp(cmd, "env") == 0) {
			system("env");
			continue;
		}

		token = strtok(cmd, " ");
		if (token) {
			char *args[] = {token, NULL};
			char *full_path = malloc(sizeof(char) * (strlen(path) + strlen(token) + 2));
			if (full_path == NULL) {
				perror("malloc");
				exit(EXIT_FAILURE);
			}
			while (token != NULL) {
				token = strtok(NULL, " ");
			}
			sprintf(full_path, "%s/%s", path, token);
			if (access(full_path, X_OK) == 0) {
				execv(full_path, args);
			} else {
				printf("%s: Command not found.\n", token);
			}
			free(full_path);
		}
		free(cmd);
	}

	return (0);
}
