#include "lst.h"
#include "utils.h"
#include "parse.h"
#include "cmd.h"
#include "files.h"
#include "str.h"

static pid_t	run_command(t_shell *shell, char *line)
{
	t_cmd	*cmd;
	int		origin_fd[2];
	pid_t	last_pid;

	origin_fd[0] = STDIN_FILENO;
	origin_fd[1] = STDOUT_FILENO;
	if (line == NULL)
		cmd = ft_parse(&shell->env);
	else
		cmd = ft_parse_line(line, &shell->env, E_FALSE);
	last_pid = 0;
	if (cmd != NULL)
	{
		last_pid = ft_exe(cmd, &shell->env, origin_fd, E_FALSE);
		ft_wait_pid(last_pid, E_TRUE);
	}
	cmd_free(cmd);
	return (last_pid);
}

static t_bool	is_sequence_separator(char *script, size_t i)
{
	size_t	j;
	int		quote_flag;
	int		depth;

	j = 0;
	depth = 0;
	quote_flag = 0;
	while (j <= i)
	{
		if (quote_update(&quote_flag, script[j]) == E_FALSE)
		{
			switch (script[j])
			{
				case '(':
					if (quote_flag == 0)
						depth++;
					break ;
				case ')':
					if (quote_flag == 0 && depth > 0)
						depth--;
					break ;
				default:
					break ;
			}
		}
		j++;
	}
	return (script[i] == ';' && quote_flag == 0 && depth == 0);
}

static int	run_script(t_shell *shell, char *script)
{
	size_t	i;
	size_t	start;
	char	*line;

	i = 0;
	start = 0;
	while (1)
	{
		if (script[i] == '\0' || is_sequence_separator(script, i) == E_TRUE)
		{
			line = ft_substr(script + start, i - start);
			run_command(shell, line);
			free(line);
			if (script[i] == '\0')
				break ;
			start = i + 1;
		}
		i++;
	}
	return (shell->status);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	if (argc != 1 && !(argc == 3 && ft_strcmp(argv[1], "-c") == 0))
		return (ft_error(F_ERROR_ARG, NULL, E_EXIT_FAILURE));
	env_init(&shell.env, envp);
	shell.status = E_EXIT_SUCCESS;
	shell.interactive = (argc == 1);
	ft_shell_set_current(&shell);
	if (shell.interactive == E_FALSE)
	{
		run_script(&shell, argv[2]);
		env_free(&shell.env);
		return (shell.status);
	}
	ft_terminal_name(E_TRUE);
	atexit(ft_terminal_restore);
	while (1)
		run_command(&shell, NULL);
	return (E_EXIT_SUCCESS);
}
