/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 04:28:02 by hyunghki          #+#    #+#             */
/*   Updated: 2023/12/15 17:05:17 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_H
# define CMD_H

# include "lst.h"
# include "utils.h"

typedef struct s_cmd
{
	int			cmd_info;
	union u_cmd	*cmd;
}	t_cmd;

typedef struct s_pu_cmd
{
	t_lst	*redir;
	t_lst	*argv;
}	t_pu_cmd;

typedef struct s_su_cmd
{
	t_lst	*redir;
	t_cmd	*target;
}	t_su_cmd;

typedef struct s_pb_cmd
{
	t_cmd	*target;
	t_cmd	*nxt;
}	t_pb_cmd;

typedef struct s_bi_cmd
{
	t_cmd	*left;
	t_cmd	*right;
}	t_bi_cmd;

union u_cmd
{
	t_pu_cmd	pu_cmd;
	t_su_cmd	su_cmd;
	t_pb_cmd	pb_cmd;
	t_bi_cmd	bi_cmd;
};

typedef enum e_exe_status
{
	E_EXE_MAIN,
	E_EXE_SUB,
	E_EXE_PIPE
}	t_exe_status;

void	chk_access_and_exe(char *path, char **argv, char **env);
void	ft_exe_rel(char *path, char **av, char **env);
pid_t	ft_fork(void);
void	ft_wait_pid(pid_t target_pid, t_bool is_main);
pid_t	ft_exe_cmd(char **av, t_lst *ev, int context);
void	pipe_exe(t_cmd *cmd, t_lst *ev, int target_fd[2], int fd[2]);
void	cmd_push(t_cmd **target, t_cmd *to_push);
t_cmd	*mk_su_cmd(t_cmd *target);
t_cmd	*mk_pb_cmd(t_cmd *target);
t_cmd	*mk_bi_cmd(t_cmd *left, t_cmd *right, int cmd_info);
t_cmd	*mk_pu_cmd(void);
void	*cmd_free(t_cmd *target);
pid_t	ft_exe(t_cmd *_this, t_lst *ev, int origin_fd[2], int exe_context);
void	fd_init(int fd[2], int fd_in, int fd_out);
t_cmd	*nxt_cmd(t_cmd *cmd);

#endif
