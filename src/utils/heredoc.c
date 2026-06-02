#include "files.h"
#include "str.h"
#include "parse.h"
#include "utils.h"
#include "lst.h"
#include "cmd.h"

static void	ft_parse_heredoc_env(int fd, char **to_read, t_env *env)
{
	char	*target;
	char	*target_val;
	char	tmp;
	int		i;
	t_bool	is_status;

	i = 0;
	(*to_read)++;
	while ((*to_read)[i])
	{
		if (ft_str_find("()|& \t\n><$\'\"*", (*to_read)[i]) != 13)
			break ;
		i++;
	}
	tmp = (*to_read)[i];
	(*to_read)[i] = '\0';
	target_val = *to_read;
	is_status = (ft_strcmp(target_val, "?") == 0);
	if (is_status == E_TRUE)
		target = ft_itoa(ft_shell_status(), 1);
	else
		target = env_get(env, target_val);
	(*to_read)[i] = tmp;
	if (target != NULL)
	{
		write(fd, target, ft_strlen(target));
		if (is_status == E_TRUE)
			free(target);
	}
	(*to_read) += i;
}

static char	*read_heredoc_line(void)
{
	char	*line;
	size_t	capacity;
	ssize_t	size;

	if (ft_shell_is_interactive() == E_TRUE)
		return (readline("\e[34mheredoc>\e[0m \e[s"));
	line = NULL;
	capacity = 0;
	size = getline(&line, &capacity, stdin);
	if (size < 0)
	{
		free(line);
		return (NULL);
	}
	if (size > 0 && line[size - 1] == '\n')
		line[size - 1] = '\0';
	return (line);
}

static void	parse_heredoc(int fd, char *del, t_bool is_trans, t_env *env)
{
	char	*tmp;
	char	*target;

	while (1)
	{
		ft_signal(heredoc_signal_handler, SIG_IGN, 0);
		target = read_heredoc_line();
		if (target == NULL && ft_shell_is_interactive() == E_TRUE)
			ft_putstr_fd("\e[u\e[B\e[A", 2);
		if (target == NULL)
			break ;
		if (ft_strcmp(del, target) == 0)
		{
			free(target);
			break ;
		}
		tmp = target;
		while (*tmp)
		{
			if (*tmp == '$' && is_trans)
				ft_parse_heredoc_env(fd, &tmp, env);
			else
				write(fd, tmp++, 1);
		}
		ft_putstr_fd("\n", fd);
		free(target);
	}
	close(fd);
	exit(E_EXIT_SUCCESS);
}

static int	heredoc_parent_wait(int fd, char *file_name)
{
	int	flag;

	waitpid(-1, &flag, 0);
	if (flag != 0)
	{
		ft_shell_set_status(1);
		close(fd);
		unlink(file_name);
		free(file_name);
		return (E_EXIT_FAILURE);
	}
	ft_signal(sigint_handler, SIG_IGN, 0);
	close(fd);
	return (E_EXIT_SUCCESS);
}

static int	create_heredoc_file(char *file_name, int fd, char *tmp, t_env *env)
{
	char	*del;
	pid_t	pid;
	t_bool	is_trans;

	is_trans = E_TRUE;
	del = delete_quote(tmp, &is_trans);
	free(tmp);
	ft_signal(SIG_IGN, SIG_IGN, 0);
	pid = ft_fork();
	if (pid == 0)
		parse_heredoc(fd, del, is_trans, env);
	free(del);
	return (heredoc_parent_wait(fd, file_name));
}

t_lst	*ft_heredoc(char *del, t_env *env)
{
	char	*file_name;
	int		fd;

	file_name = ft_strdup(F_HEREDOC_TEMPLATE);
	fd = mkstemp(file_name);
	if (fd < 0)
	{
		free(file_name);
		exit(ft_error(F_ERROR_HEREDOC_FILE, NULL, E_EXIT_FAILURE));
	}
	if (create_heredoc_file(file_name, fd, del, env) != E_EXIT_SUCCESS)
		return (NULL);
	return (mk_lst(file_name, E_T_HEREDOC));
}
