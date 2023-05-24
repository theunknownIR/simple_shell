#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define D_PROMPT "$ "

void execute_command(char *cmd, char *path);
void print_env(void);
#endif
