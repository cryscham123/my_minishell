/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 14:17:05 by hyunghki          #+#    #+#             */
/*   Updated: 2023/12/14 19:49:12 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "files.h"
#include "str.h"
#include "parse.h"
#include "utils.h"
#include "lst.h"
#include "cmd.h"

extern int	g_status;

static void	ft_parse_heredoc_env(int fd, char **to_read, t_lst *ev)
{
	t_lst	*to_find;
	char	*target;
	char	tmp;
	int		i;

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
	to_find = ft_env_find(ev, (*to_read));
	(*to_read)[i] = tmp;
	if (to_find != NULL)
	{
		target = ((char *)to_find->data) + (to_find->info + 1);
		write(fd, target, ft_strlen(target));
	}
	(*to_read) += i;
}

static void	parse_heredoc(int fd, char *del, t_bool is_trans, t_lst *ev)
{
	char	*tmp;
	char	*target;

	while (1)
	{
		ft_signal(heredoc_signal_handler, SIG_IGN, 0);
		target = readline("\e[34mheredoc>\e[0m \e[s");
		if (target == NULL)
			ft_putstr_fd("\e[u\e[B\e[A", 2);
		if (target == NULL || ft_strcmp(del, target) == 0)
			break ;
		tmp = target;
		while (*tmp)
		{
			if (*tmp == '$' && is_trans)
				ft_parse_heredoc_env(fd, &tmp, ev);
			else
				write(fd, tmp++, 1);
		}
		ft_putstr_fd("\n", fd);
		free(target);
	}
	exit(E_EXIT_SUCCESS);
}

static int	heredoc_parent_wait(int fd, char *file_name)
{
	int	flag;

	waitpid(-1, &flag, 0);
	if (flag != 0)
	{
		g_status = 1;
		close(fd);
		unlink(file_name);
		free(file_name);
		return (E_EXIT_FAILURE);
	}
	ft_signal(sigint_handler, SIG_IGN, 0);
	close(fd);
	return (E_EXIT_SUCCESS);
}

static int	create_heredoc_file(char *file_name, char *tmp, t_lst *ev)
{
	char	*del;
	int		fd;
	pid_t	pid;
	t_bool	is_trans;

	is_trans = E_TRUE;
	del = delete_quote(tmp, &is_trans);
	free(tmp);
	fd = open(file_name, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd < 0)
		exit(ft_error(F_ERROR_HEREDOC_FILE, NULL, E_EXIT_FAILURE));
	ft_signal(SIG_IGN, SIG_IGN, 0);
	pid = ft_fork();
	if (pid == 0)
		parse_heredoc(fd, del, is_trans, ev);
	free(del);
	return (heredoc_parent_wait(fd, file_name));
}

t_lst	*ft_heredoc(char *del, t_lst *ev)
{
	char	*file_name;
	char	*tmp;
	size_t	heredoc_num;

	heredoc_num = 0;
	if (access(F_HEREDOC_PREFIX, F_OK) != 0)
		exit(ft_error(F_ERROR_HEREDOC_SET, F_HEREDOC_PREFIX, E_EXIT_FAILURE));
	while (heredoc_num < HEREDOC_MAX_FILE_NAME)
	{
		tmp = ft_itoa(++heredoc_num, 1);
		file_name = ft_append(F_HEREDOC_PREFIX, tmp, '/');
		free(tmp);
		if (access(file_name, F_OK) != 0)
			break ;
		free(file_name);
	}
	if (heredoc_num == HEREDOC_MAX_FILE_NAME)
		exit(ft_error(F_ERROR_HEREDOC_FILE, NULL, E_EXIT_FAILURE));
	if (create_heredoc_file(file_name, del, ev) != E_EXIT_SUCCESS)
		return (NULL);
	return (mk_lst(file_name, E_T_HEREDOC));
}
