/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 14:09:31 by hyunghki          #+#    #+#             */
/*   Updated: 2023/12/14 19:57:34 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd.h"
#include "lst.h"
#include "files.h"
#include "parse.h"
#include "utils.h"
#include "str.h"

int	ft_open(int target_fd[2], int fd[2], char *s, int info)
{
	int			new_fd;
	int			direction;

	if (info == E_T_INPUT || info == E_T_HEREDOC)
		new_fd = open(s, O_RDONLY);
	else if (info == E_T_OUTPUT)
		new_fd = open(s, O_CREAT | O_RDWR | O_TRUNC, 0644);
	else
		new_fd = open(s, O_CREAT | O_RDWR | O_APPEND, 0644);
	if (new_fd < 0)
		return (ft_error(F_FILE_FAIL, s, E_EXIT_FILE_NOT_EXIST));
	direction = (info == E_T_OUTPUT || info == E_T_APPEND);
	if (target_fd[direction] != fd[direction])
		close(target_fd[direction]);
	target_fd[direction] = new_fd;
	return (E_EXIT_SUCCESS);
}

void	ft_close(int fd[2], int origin_fd[2])
{
	if (fd[0] != origin_fd[0])
		close(fd[0]);
	if (fd[1] != origin_fd[1])
		close(fd[1]);
}

void	ft_dup(int *fd, int *old_fd, t_dup dup_info)
{
	if (dup_info == E_DUP)
	{
		fd[0] = dup(old_fd[0]);
		fd[1] = dup(old_fd[1]);
		if (fd[0] < 0 || fd[1] < 0)
			exit(ft_error(F_ERROR_SYS, "dup", E_EXIT_FAILURE));
	}
	else
	{
		if (dup2(fd[0], old_fd[0]) < 0)
			exit(ft_error(F_ERROR_SYS, "dup2", E_EXIT_FAILURE));
		if (dup2(fd[1], old_fd[1]) < 0)
			exit(ft_error(F_ERROR_SYS, "dup2", E_EXIT_FAILURE));
	}
}

void	ft_pipe(int fd[2])
{
	if (pipe(fd) < 0)
		exit(ft_error(F_ERROR_SYS, "pipe", E_EXIT_FAILURE));
}

int	ft_redirection(int target_fd[2], int fd[2], t_lst *redir, t_lst *ev)
{
	int		flag;
	char	*target;
	t_lst	*translated;

	flag = E_EXIT_SUCCESS;
	while (redir != NULL && flag == E_EXIT_SUCCESS)
	{
		translated = NULL;
		if (redir->info != E_T_HEREDOC)
		{
			translated = ft_expansion(redir->data, ev);
			if (translated == NULL || translated->nxt != NULL)
				flag = ft_error(F_ERROR_AMBIG, redir->data, E_EXIT_FAILURE);
			else
				target = translated->data;
		}
		else
			target = redir->data;
		if (flag == 0)
			flag = ft_open(target_fd, fd, target, redir->info);
		ft_lst_free(translated);
		redir = redir->nxt;
	}
	return (flag);
}
