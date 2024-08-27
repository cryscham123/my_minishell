/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 16:45:42 by hyunghki          #+#    #+#             */
/*   Updated: 2023/12/15 19:03:55 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd.h"
#include "parse.h"
#include "files.h"
#include "lst.h"

extern int	g_status;

void	fd_init(int fd[2], int fd_in, int fd_out)
{
	fd[0] = fd_in;
	fd[1] = fd_out;
}

void	pipe_exe(t_cmd *cmd, t_lst *ev, int target_fd[2], int fd[2])
{
	pid_t	last_pid;

	last_pid = ft_exe(cmd, ev, target_fd, E_EXE_PIPE);
	ft_close(target_fd, fd);
	ft_wait_pid(last_pid, E_FALSE);
	exit(g_status);
}

t_cmd	*nxt_cmd(t_cmd *cmd)
{
	if (cmd == NULL)
		return (NULL);
	if (cmd->cmd_info == 0)
		return (NULL);
	if (cmd->cmd_info == E_T_SUB)
		return (cmd->cmd->su_cmd.target);
	if (cmd->cmd_info == E_T_PIPE)
		return (cmd->cmd->pb_cmd.nxt);
	return (cmd->cmd->bi_cmd.right);
}

void	cmd_push(t_cmd **target, t_cmd *to_push)
{
	t_cmd	*tmp;
	t_cmd	*next;

	if (*target == NULL)
	{
		*target = to_push;
		return ;
	}
	tmp = *target;
	while (tmp != NULL)
	{
		next = nxt_cmd(tmp);
		if (next == NULL)
		{
			if (tmp->cmd_info == E_T_SUB)
				tmp->cmd->su_cmd.target = to_push;
			else if (tmp->cmd_info == E_T_PIPE)
				tmp->cmd->pb_cmd.target = to_push;
			else
				tmp->cmd->bi_cmd.right = to_push;
			return ;
		}
		tmp = next;
	}
}

void	*cmd_free(t_cmd *target)
{
	if (target == NULL)
		return (NULL);
	if (target->cmd_info == 0)
	{
		ft_lst_free(target->cmd->pu_cmd.argv);
		ft_lst_free(target->cmd->pu_cmd.redir);
	}
	else if (target->cmd_info == E_T_SUB)
	{
		cmd_free(target->cmd->su_cmd.target);
		ft_lst_free(target->cmd->su_cmd.redir);
	}
	else if (target->cmd_info == E_T_PIPE)
	{
		cmd_free(target->cmd->pb_cmd.target);
		cmd_free(target->cmd->pb_cmd.nxt);
	}
	else
	{
		cmd_free(target->cmd->bi_cmd.left);
		cmd_free(target->cmd->bi_cmd.right);
	}
	free(target->cmd);
	free(target);
	return (NULL);
}
