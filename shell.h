#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

extern char **environ;

char *find_cmd(const char *cmd, char *fullpath, const char *path);
int cmp_str(const char *s1, const char *s2);
char *dup_str(const char *src);
unsigned int str_len(const char *s);
void print_env(void);
char *get_env(const char *name);
void error(const char *name, int counter, const char *cmd);
int is_whitespace(const char *c);
char **make_tokens(const char *string);
void handle_EOF(const char *buffer);
int execute_cmd(char **tokens, const char *av, const char *path);
void p_error(const char *av, const char *num);
int atoi(const char *s);

#endif /* SHELL_H */
