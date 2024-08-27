/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 17:28:20 by hyunghki          #+#    #+#             */
/*   Updated: 2023/12/14 16:05:02 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"
#include "files.h"
#include "utils.h"
#include "str.h"

t_bool	echo_option_chk(char *s)
{
	int	i;

	if (s == NULL)
		return (E_FALSE);
	i = 1;
	if (s[0] != '-' || s[1] == '\0')
		return (E_FALSE);
	while (s[i] == 'n')
		i++;
	return (s[i] == '\0');
}

int	ft_av_validate_dir(char **s)
{
	struct stat	buf;

	stat(*s, &buf);
	if (access(*s, F_OK) != 0)
		return (ft_error(F_ERROR_FILE, *s, E_BUILT_IN_FAIL));
	if (access(*s, X_OK) != 0)
		return (ft_error(F_ERROR_ACCESS, *s, E_BUILT_IN_FAIL));
	if (!S_ISDIR(buf.st_mode))
		return (ft_error(F_ERROR_NO_DIR, *s, E_BUILT_IN_FAIL));
	return (E_BUILT_IN_SUCCESS);
}

int	ft_set_ev_pwd(char *key, t_lst *ev)
{
	char	*pwd;
	char	*target;
	t_lst	*to_push;

	pwd = getcwd(NULL, 0);
	if (pwd == NULL)
		return (ft_error(F_ERROR_FILE, NULL, E_BUILT_IN_FAIL));
	target = ft_append(key, pwd, '=');
	free(pwd);
	to_push = mk_lst(target, ft_strlen(key));
	ft_unset_unit(target, ev, 0);
	lst_push(&ev, to_push);
	return (0);
}

int	ft_av_validate_ev(char *s)
{
	int	i;

	if (*s >= '0' && *s <= '9')
		return (ft_error(F_ERROR_KEY, s, E_EV_KEY_ERROR));
	i = 0;
	while (s[i] && s[i] != '=')
	{
		if (!(i != 0 && s[i] >= '0' && s[i] <= '9') \
			&& !(s[i] >= 'a' && s[i] <= 'z') \
			&& !(s[i] >= 'A' && s[i] <= 'Z') && s[i] != '_')
			return (ft_error(F_ERROR_KEY, s, E_EV_KEY_ERROR));
		i++;
	}
	if (i == 0)
		return (ft_error(F_ERROR_KEY, s, E_EV_KEY_ERROR));
	if (s[i] != '=')
		return (E_EV_NO_VALUE);
	return (0);
}

void	ft_unset_unit(char *s, t_lst *ev, int flag)
{
	int		i;

	i = 0;
	if (flag != E_EV_NO_VALUE)
	{
		i = ft_str_find(s, '=');
		s[i] = '\0';
	}
	lst_delete(ft_env_find(ev, s));
	if (flag != E_EV_NO_VALUE)
		s[i] = '=';
}
