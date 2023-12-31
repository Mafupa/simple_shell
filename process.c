#include "main.h"

/**
 * find_path - find a program's path in the PATH environement variable
 * @program_name: program's name
 * @prgm_name: the name of the shell program
 *
 * Return: the programs full path
 */
char *find_path(char *program_name, char *prgm_name)
{
	int i = 0, j = 0, c, d, pn_len = strlen(program_name), path_len;
	char *PATH, *attempt;

	PATH = _getenv("PATH");
	if (!PATH)
		return (NULL);
	path_len = strlen(PATH);
	for (i = 0; i < path_len; i++)
	{
		if (PATH[i] != ':' && PATH[i] != '\n')
			continue;
		attempt = malloc(sizeof(char) * (i - j + pn_len + 2));
		if (!attempt)
			return (NULL);
		for (c = 0; c < (i - j); c++)
			attempt[c] = PATH[j + c];
		attempt[c] = '/';
		c++;
		for (d = 0; d < pn_len; d++)
			attempt[c + d] = program_name[d];
		attempt[c + d] = '\0';
		i++;
		j = i;
		if (access(attempt, F_OK) != 0)
			free(attempt);
		else
			return (attempt);
	}
	/*perror(prgm_name);*/
	fprintf(stderr, "%s: 1: %s: not found\n", prgm_name, program_name);
	return (NULL);
}

/**
 * _getenv -  get a environement variable
 * @name: the environement variable's title
 *
 * Return: the value of the environement valiable
 */
char *_getenv(char *name)
{
	int name_len = 0;
	char **env;

	if (!name || !environ)
		return (NULL);
	while (name[name_len])
		name_len++;
	for (env = environ; *env != NULL; env++)
	{
		if (strncmp(*env, name, name_len) == 0 && (*env)[name_len] == '=')
			return (&(*env)[name_len + 1]);
	}
	return (NULL);
}

/**
 * start_process - starts a process
 * @args: arguments given
 * @prgm_name: program's name
 * Return: 1
 */
int start_process(char **args, char *prgm_name)
{
	pid_t pid;
	int status;
	char *path;

	if (access(args[0], F_OK) == 0)
		path = strdup(args[0]);
	else
		path = find_path(args[0], prgm_name);
	if (!path)
		return (127);

	pid = fork();
	if (pid == 0)
	{
		if (execve(path, args, environ) == -1)
			perror(prgm_name);
		exit(EXIT_FAILURE);
	}
	else if (pid < 0)
	{
		perror(prgm_name);
	}
	else
	{
		do {
			waitpid(pid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}
	free(path);
	return (1);
}
