#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024

extern char **environ;

char *findCmdPath(char *cmd, char *fullpath, char *path);
int compareStrings(char *s1, char *s2);
char *cpyStr(char *dest, char *src);
char *getEnvVar(char *name);
void handleEOF(char *buf);
int executeCommand(char **tokens, char *av, char *path);
int strToInt(char *s);
void printError(char *name, int counter, char *cmd);
int isWhitespace(char *c);
unsigned int strLen(char *s);
void printEnv(void);
char **tokenizeStr(char *str);
char *my_getline();
void printErrorMessage(char *av, char *number);


/**
 * main - entry point to shell
 * @argc: arg number
 * @av: arg value
 * Return: -1 | 1
 */
int main(int __attribute__((unused)) argc, char *av[])
{
    char *pathEnv, *path, **tokens, st = 0;
    int flag = 0, len = 0;

    while (1)
    {
        char *buffer;
        if (isatty(0))
            write(STDOUT_FILENO, "$ ", 2);
        buffer = my_getline();
        if (buffer == NULL)
            continue;
        if (isWhitespace(buffer) == 1) {
            free(buffer);
            continue;
        }
        buffer[len - 1] = '\0';
        if (isWhitespace(buffer) == 1)
            continue;
        tokens = tokenizeStr(buffer);
        if (compareStrings(tokens[0], "exit") == 0)
        {
            free(buffer), free(tokens), exit(st);
        }
        if (compareStrings(tokens[0], "env") == 0)
        {
            free(tokens), printEnv();
            continue;
        }
        pathEnv = getEnvVar("PATH"), path = findCmdPath(tokens[0], path, pathEnv);
        if (path == NULL)
            path = tokens[0];
        else
            flag = 1;
        st = executeCommand(tokens, av[0], path), free(buffer);
        buffer = NULL, free(tokens);
        if (flag == 1)
            free(path);
    }

    return (0);
}


void printErrorMessage(char *av, char *number)
{
    write(STDERR_FILENO, av, strLen(av));
    write(STDERR_FILENO, ": 1: exit: ", strLen(": 1: exit: "));
    write(STDERR_FILENO, number, strLen(number));
    write(STDERR_FILENO, ": numeric argument required\n", strLen(": numeric argument required\n"));
}

/**
 * compareStrings - Compare two strings
 * @s1: string 1
 * @s2: string 2
 * Return: comparison result
 */
int compareStrings(char *s1, char *s2)
{
    int i;

    for (i = 0; s1[i] != '\0' && s1[i] == s2[i]; i++)
    {
    }
    return (s1[i] - s2[i]);
}

/**
 * cpyStr - Copy a string
 * @dest: destination string
 * @src: source string
 * Return: pointer to destination string
 */
char *cpyStr(char *dest, char *src)
{
    int i;

    for (i = 0; src[i] != '\0'; i++)
    {
        dest[i] = src[i];
    }
    dest[i] = '\0';

    return (dest);
}

/**
 * strLen - Calculate the length of a string
 * @s: input string
 * Return: length of the string
 */
unsigned int strLen(char *s)
{
    unsigned int length = 0;

    while (s[length])
        length++;

    return (length);
}

/**
 * executeCommand - Execute a command
 * @tokens: array of tokens
 * @av: argument value
 * @path: command path
 * Return: command exit status
 */
int executeCommand(char **tokens, char *av, char *path)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid < 0)
    {
        perror(av);
        exit(1);
    }
    else if (pid == 0)
    {
        if (execve(path, tokens, environ) == -1)
        {
            perror(av);
            exit(2);
        }
    }
    else
        wait(&status);
    return (WEXITSTATUS(status));
}

/**
 * envNameCompare - Compare an environment variable name
 * @env: environment variable
 * @name: variable name
 * Return: number of characters in common between name and env
 */
int envNameCompare(char *env, char *name)
{
    int i = 0;

    while (env[i] != '=')
    {
        if (env[i] != name[i])
            return (0);
        i++;
    }
    return (i + 1);
}

/**
 * printEnv - Print the environment variables
 */
void printEnv(void)
{
    size_t i = 0, length = 0;

    while (environ[i])
    {
        length = strLen(environ[i]);
        write(STDOUT_FILENO, environ[i], length);
        write(STDOUT_FILENO, "\n", 1);
        i++;
    }
}

/**
 * getEnvVar - Get the value of an environment variable
 * @name: name of the environment variable
 * Return: pointer to the value of the environment variable
 */
char *getEnvVar(char *name)
{
    char *result = NULL;
    int i = 0, move = 0;

    while (environ[i] != NULL)
    {
        move = envNameCompare(environ[i], name);

        if (move != 0)
        {
            result = environ[i];
            break;
        }
        i++;
    }
    return (result + move);
}

/**
 * findCmdPath - Find the full path of a command
 * @cmd: command name
 * @fullpath: buffer to store full path
 * @path: PATH environment variable
 * Return: full path of the command
 */
char *findCmdPath(char *cmd, char *fullpath, char *path)
{
    unsigned int cmdLen, pathLen;
    char *pathCopy, *token;

    cmdLen = strLen(cmd);

    pathCopy = malloc(sizeof(char) * strLen(path) + 1);
    cpyStr(pathCopy, path);
    token = strtok(pathCopy, ":");

    while (token != NULL)
    {
        pathLen = strLen(token);
        fullpath = malloc(sizeof(char) * (pathLen + cmdLen) + 2);
        if (fullpath == NULL)
            return (NULL);

        cpyStr(fullpath, token);
        fullpath[pathLen] = '/';
        cpyStr(fullpath + pathLen + 1, cmd);
        fullpath[pathLen + cmdLen + 1] = '\0';
        if (access(fullpath, X_OK) != 0)
        {
            free(fullpath);
            fullpath = NULL;
            token = strtok(NULL, ":");
        }
        else
            break;
    }
    free(pathCopy);
    return (fullpath);
}

/**
 * strToInt - Convert a string to an integer
 * @s: input string
 * Return: converted integer
 */
int strToInt(char *s)
{
    int sign = 1;
    int result = 0;

    while (*s != '\0')
    {
        if (*s == '-')
        {
            sign = -1;
        }
        else if (*s >= '0' && *s <= '9')
        {
            result = result * 10 + (*s - '0');
        }
        else if (result > 0)
        {
            break;
        }
        s++;
    }

    return (sign * result);
}

/**
 * isWhitespace - Check if a character is whitespace
 * @c: input character
 * Return: 1 if whitespace, 0 otherwise
 */
int isWhitespace(char *c)
{
    int i;
    char s;

    for (i = 0; c[i] != '\0'; i++)
    {
        s = c[i];
        if (s != '\t' && s != ' ')
            return (0);
    }
    return (1);
}

/**
 * tokenizeStr - Tokenize a string
 * @string: input string
 * Return: array of tokens
 */
char **tokenizeStr(char *string)
{
    char **tokens;
    char *token;
    int i = 0;

    tokens = (char **)malloc(sizeof(char) * 1024);
    if (tokens == NULL)
    {
        exit(1);
    }
    token = strtok(string, "\n\t\r ");
    if (token == NULL)
        return (NULL);
    while (token != NULL)
    {
        tokens[i++] = token;
        token = strtok(NULL, "\n\t\r ");
    }
    tokens[i] = NULL;

    return (tokens);
}

/**
 * handleEOF - Handle end-of-file condition
 * @buffer: input buffer
 */
void handleEOF(char *buffer)
{
    if (buffer)
    {
        free(buffer);
        buffer = NULL;
    }

    if (isatty(STDIN_FILENO))
        write(STDOUT_FILENO, "\n", 1);
    free(buffer);
    exit(0);
}

char *my_getline() {
    char *line = NULL;
    size_t bufsize = 0;
    ssize_t len = 0;
    int c;

    while ((c = getchar()) != EOF && c != '\n') {
        if ((size_t)(len + 1) >= bufsize) {
            bufsize += BUFFER_SIZE;
            line = realloc(line, bufsize);
            if (!line) {
                perror("my_getline");
                exit(1);
            }
        }
        line[len++] = (char) c;
    }

    if (len == 0 && c == EOF) {
        free(line);
        exit(0);
    }

    if ((size_t)(len + 1) >= bufsize) {
        bufsize += 1;
        line = realloc(line, bufsize);
        if (!line) {
            perror("my_getline");
            exit(1);
        }
    }
    
    line[len] = '\0';
    return line;
}
