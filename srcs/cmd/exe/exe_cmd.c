/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 17:28:20 by hyunghki          #+#    #+#             */
/*   Updated: 2023/12/14 17:32:17 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd.h"
#include "str.h"
#include "utils.h"
#include "files.h"
#include "built_in.h"

static int	ft_exe_built_in(char **argv, t_lst *ev, t_bool is_forked)
{
	int		flag;

	flag = E_BUILT_IN_NO_BUILT;
	if (ft_strcmp(argv[0], "echo") == 0)
		flag = ft_echo(argv + 1);
	else if (ft_strcmp(argv[0], "cd") == 0)
		flag = ft_cd(argv + 1, ev);
	else if (ft_strcmp(argv[0], "pwd") == 0)
		flag = ft_pwd();
	else if (ft_strcmp(argv[0], "export") == 0)
		flag = ft_export(argv + 1, ev);
	else if (ft_strcmp(argv[0], "unset") == 0)
		flag = ft_unset(argv + 1, ev);
	else if (ft_strcmp(argv[0], "env") == 0)
		flag = ft_env(ev->nxt->nxt);
	else if (ft_strcmp(argv[0], "exit") == 0)
		flag = ft_exit(argv + 1, is_forked);
	return (flag);
}

static void	ft_exe_extern(char **av, char **env, t_lst *ev)
{
	t_lst	*path;
	char	*target;

	path = ft_env_find(ev, "PATH");
	if (path == NULL || ft_str_find(av[0], '/') < ft_strlen(av[0]))
	{
		chk_access_and_exe(av[0], av, env);
		exit(ft_error(F_ERROR_FILE, av[0], E_EXIT_NO_CMD));
	}
	ft_signal(SIG_DFL, SIG_DFL, 1);
	target = path->data + (path->info + 1);
	ft_exe_rel(target, av, env);
}

pid_t	ft_exe_cmd(char **av, t_lst *ev, int context)
{
	char	**env;
	pid_t	last_pid;

	last_pid = ft_exe_built_in(av, ev, (context != E_EXE_MAIN));
	if (last_pid == E_BUILT_IN_NO_BUILT)
	{
		env = mk_argv(ev->nxt->nxt);
		if (context != E_EXE_PIPE)
		{
			ft_signal(SIG_IGN, SIG_IGN, 0);
			last_pid = ft_fork();
			if (last_pid == 0)
				ft_exe_extern(av, env, ev);
		}
		else
			ft_exe_extern(av, env, ev);
		ft_argv_free(env);
	}
	return (last_pid);
}
