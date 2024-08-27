/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 17:28:20 by hyunghki          #+#    #+#             */
/*   Updated: 2023/12/14 16:08:34 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd.h"
#include "files.h"
#include "utils.h"
#include "str.h"

void	chk_access_and_exe(char *path, char **argv, char **env)
{
	char		*target;
	t_bool		is_dir;
	struct stat	buf;

	is_dir = (path[ft_strlen(path) - 1] == '/');
	if (is_dir)
		target = ft_substr(path, ft_strlen(path) - 1);
	else
		target = path;
	if (access(target, F_OK) != 0)
	{
		if (is_dir)
			free(target);
		return ;
	}
	stat(target, &buf);
	if (is_dir && !S_ISDIR(buf.st_mode))
		exit(ft_error(F_ERROR_NO_DIR, path, E_CMD_NO_FILE));
	if (!S_ISREG(buf.st_mode))
		exit(ft_error(F_ERROR_NO_FILE, path, E_CMD_NO_FILE));
	if (access(target, X_OK) != 0)
		exit(ft_error(F_ERROR_ACCESS, path, E_CMD_ACCESS));
	execve(target, argv, env);
}

void	ft_exe_rel(char *path, char **av, char **env)
{
	char	*target_path;
	t_bool	is_end;
	size_t	i;

	if (*av[0] == '\0')
		exit(ft_error(F_ERROR_EXE, av[0], E_EXIT_NO_CMD));
	is_end = E_FALSE;
	while (!is_end)
	{
		i = ft_str_find(path, ':');
		is_end = (path[i] == '\0');
		path[i] = '\0';
		target_path = ft_append(path, av[0], '/');
		chk_access_and_exe(target_path, av, env);
		free(target_path);
		if (is_end)
			break ;
		path += (i + 1);
	}
	exit(ft_error(F_ERROR_EXE, av[0], E_EXIT_NO_CMD));
}
