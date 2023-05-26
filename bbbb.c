#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

extern char **environ;

char *searchCommand(const char *command, char *fullPath, const char *searchPath);
int compareStrings(const char *str1, const char *str2);
char *duplicateString(const char *src);
unsigned int getStringLength(const char *str);
void printEnvironment(void);
char *getEnvironmentVariable(const char *name);
void handleError(const char *program, int counter, const char *command);
int isWhitespace(const char *c);
char **tokenizeString(const char *string);
void handleEOF(const char *buffer);
int executeCommand(char **tokens, const char *programName, const char *path);
int convertToInt(const char *str);

void printError(const char *programName, const char *number)
{
	printf("%s: 1: exit: Illegal number: %s\n", programName, number);
}

int main(int __attribute__((unused)) argc, char *argv[])
{
	char *buffer = NULL, *pathEnv, *path, **tokens;
	size_t bufferSize = 0;
	int status = 0, length = 0;

	while (1)
	{
		if (isatty(0))
			printf("$ ");
		length = getline(&buffer, &bufferSize, stdin);
		if (length == -1)
		{
			free(buffer);
			exit(0);
		}
		if (length == EOF)
			handleEOF(buffer);
		if (*buffer == '\n')
		{
			free(buffer);
			continue;
		}
		buffer[length - 1] = '\0';
		if (isWhitespace(buffer) == 1)
			continue;
		tokens = tokenizeString(buffer);
		if (compareStrings(tokens[0], "exit") == 0)
		{
			free(buffer);
			free(tokens);
			exit(status);
		}
		if (compareStrings(tokens[0], "env") == 0)
		{
			free(tokens);
			printEnvironment();
			continue;
		}
		pathEnv = getEnvironmentVariable("PATH");
		path = searchCommand(tokens[0], path, pathEnv);
		if (path == NULL)
			path = tokens[0];
		else
			status = 1;
		status = executeCommand(tokens, argv[0], path);
		free(buffer);
		buffer = NULL;
		bufferSize = 0;
		free(tokens);
		if (status == 1)
			free(path);
	}
	return 0;
}

int compareStrings(const char *str1, const char *str2)
{
	int i;

	for (i = 0; str1[i] != '\0' && str1[i] == str2[i]; i++)
	{
	}
	return (str1[i] - str2[i]);
}

char *duplicateString(const char *src)
{
	char *dest = malloc(strlen(src) + 1);
	if (dest == NULL)
	{
		perror("malloc");
		exit(1);
	}
	strcpy(dest, src);
	return dest;
}

unsigned int getStringLength(const char *str)
{
	unsigned int length = 0;

	while (str[length])
		length++;

	return length;
}

int executeCommand(char **tokens, const char *programName, const char *path)
{
	pid_t pid;
	int status;

	pid = fork();
	if (pid < 0)
	{
		perror(programName);
		exit(1);
	}
	else if (pid == 0)
	{
		if (execve(path, tokens, environ) == -1)
		{
			perror(programName);
			exit(2);
		}
	}
	else
		wait(&status);
	return WEXITSTATUS(status);
}

int compareEnvironmentName(const char *env, const char *name)
{
	int i = 0;

	while (env[i] != '=')
	{
		if (env[i] != name[i])
			return 0;
		i++;
	}
	return (i + 1);
}

void printEnvironment(void)
{
	size_t i = 0, length = 0;

	while (environ[i])
	{
		length = strlen(environ[i]);
		printf("%s\n", environ[i]);
		i++;
	}
}

char *getEnvironmentVariable(const char *name)
{
	int i = 0, moveTo = 0;

	while (environ[i] != NULL)
	{
		moveTo = compareEnvironmentName(environ[i], name);

		if (moveTo != 0)
		{
			return environ[i] + moveTo;
		}
		i++;
	}
	return NULL;
}

char *searchCommand(const char *command, char *fullPath, const char *searchPath)
{
	unsigned int commandLength, pathLength;
	char *pathCopy, *token;

	commandLength = strlen(command);

	pathCopy = duplicateString(searchPath);
	token = strtok(pathCopy, ":");

	while (token != NULL)
	{
		pathLength = strlen(token);
		fullPath = malloc(pathLength + commandLength + 2);
		if (fullPath == NULL)
		{
			perror("malloc");
			return NULL;
		}

		strcpy(fullPath, token);
		fullPath[pathLength] = '/';
		strcpy(fullPath + pathLength + 1, command);
		fullPath[pathLength + commandLength + 1] = '\0';
		if (access(fullPath, F_OK) == 0)
		{
			free(pathCopy);
			return fullPath;
		}

		free(fullPath);
		token = strtok(NULL, ":");
	}

	free(pathCopy);
	return NULL;
}

int isWhitespace(const char *c)
{
	int i = 0;

	while (c[i] != '\0')
	{
		if (c[i] != ' ' && c[i] != '\t' && c[i] != '\n' && c[i] != '\r')
			return 0;
		i++;
	}
	return 1;
}

char **tokenizeString(const char *string)
{
	char **tokens = NULL;
	char *token = NULL;
	int count = 0;
	char *stringCopy;

	stringCopy = duplicateString(string);
	token = strtok(stringCopy, " \t");
	while (token != NULL)
	{
		tokens = realloc(tokens, (count + 1) * sizeof(char *));
		if (tokens == NULL)
		{
			perror("realloc");
			exit(1);
		}
		tokens[count] = strdup(token);
		if (tokens[count] == NULL)
		{
			perror("strdup");
			exit(1);
		}
		count++;
		token = strtok(NULL, " \t");
	}
	tokens = realloc(tokens, (count + 1) * sizeof(char *));
	if (tokens == NULL)
	{
		perror("realloc");
		exit(1);
	}
	tokens[count] = NULL;
	free(stringCopy);
	return tokens;
}

void handleEOF(const char *buffer)
{
	if (isatty(0))
		printf("\n");
	free((void *)buffer);
	exit(0);
}

int convertToInt(const char *str)
{
	int i = 0, sign = 1, result = 0;

	if (str[0] == '-')
	{
		sign = -1;
		i++;
	}

	for (; str[i] != '\0'; i++)
	{
		if (str[i] >= '0' && str[i] <= '9')
			result = result * 10 + (str[i] - '0');
		else
			break;
	}

	return sign * result;
}
