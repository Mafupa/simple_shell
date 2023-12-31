#include "main.h"

/**
 * main - oshell
 * @argc: argument count
 * @argv: argument vector
 * Return: EXIT_SUCCESS
 */
int main(int argc __attribute__((unused)), char **argv __attribute__((unused)))
{
	char *line, *clean_line;
	char **args;
	int status = 1;

	do {
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "$ ", 1);
		line = read_line(stdin);
		clean_line = remove_useless_space(line);
		args = split_line(clean_line);
		free(line);
		free(clean_line);
		/* fun args*/
		if (strcmp(args[0], "exit") == 0)
			cmd_exit(args, argv[0]);
		else if (strcmp(args[0], "cd") == 0)
			status = cmd_cd(args, argv[0]);
		else if (strcmp(args[0], "env") == 0)
			status = cmd_env();
		else if (strcmp(args[0], "") == 0)
			;
		else
			status = start_process(args, argv[0]);
		free_string_array(args);
	} while (status);
	return (EXIT_SUCCESS);

}
