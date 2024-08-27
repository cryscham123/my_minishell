/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_lst.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 11:20:05 by hyunghki          #+#    #+#             */
/*   Updated: 2023/12/14 14:18:05 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lst.h"
#include "str.h"
#include "utils.h"

extern int	g_status;

static void	update_status_env(t_lst *ev)
{
	char	*num;

	free(ev->data);
	num = ft_itoa(g_status, 1);
	ev->data = ft_append("?=", num, '\0');
	free(num);
}

static t_lst	*increase_shlvl(char *env, size_t equal_pos)
{
	char	*target;
	char	*val;
	char	*res;
	t_lst	*ret;

	res = ft_calloc(sizeof(char) * (ft_strlen(env) + 2));
	ft_strncpy(res, env, equal_pos + 1);
	target = env + equal_pos + 1;
	if (!ft_str_is_num(target))
		val = ft_substr("1", 1);
	else
		val = ft_str_plus(target, "1", ft_strlen(target), 1);
	ft_strncpy(res + equal_pos + 1, val, ft_strlen(val));
	free(val);
	ret = mk_str_node(res, equal_pos);
	free(res);
	return (ret);
}

t_lst	*mk_ev_lst(char **env)
{
	size_t	key_size;
	t_lst	*ev;
	t_lst	*to_push;

	ev = NULL;
	lst_push(&ev, mk_str_node("?=0", 1));
	lst_push(&ev, mk_str_node("=$", 0));
	while (*env)
	{
		key_size = ft_str_find(*env, '=');
		if (ft_strncmp("SHLVL", *env, key_size) == 0)
			to_push = increase_shlvl(*env, key_size);
		else
			to_push = mk_str_node(*env, key_size);
		lst_push(&ev, to_push);
		env++;
	}
	if (ft_env_find(ev, "SHLVL") == NULL)
	{
		to_push = mk_str_node("SHLVL=1", 5);
		lst_push(&ev, to_push);
	}
	return (ev);
}

t_lst	*ft_env_find(t_lst *ev, char *s)
{
	if (s == NULL)
		return (NULL);
	if (ft_strcmp(s, "?") == 0)
		update_status_env(ev);
	while (ev != NULL)
	{
		ev->data[ev->info] = '\0';
		if (ft_strcmp(ev->data, s) == 0)
		{
			ev->data[ev->info] = '=';
			return (ev);
		}
		ev->data[ev->info] = '=';
		ev = ev->nxt;
	}
	return (NULL);
}
