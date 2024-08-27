/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 17:28:20 by hyunghki          #+#    #+#             */
/*   Updated: 2023/12/14 17:35:03 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"
#include "utils.h"

extern int	g_status;

int	ft_env(t_lst *ev)
{
	while (ev != NULL)
	{
		ft_putstr_fd(ev->data, 1);
		ft_putstr_fd("\n", 1);
		ev = ev->nxt;
	}
	return (0);
}

int	ft_exit(char **av, t_bool is_forked)
{
	int		num;

	num = g_status;
	if (*av != NULL)
	{
		num = ft_av_exit(*av, is_forked);
		if (*(av + 1) != NULL)
			return (ft_error(F_ERROR_ARG, NULL, E_BUILT_IN_FAIL));
	}
	if (is_forked == E_FALSE)
		ft_putstr_fd("exit\n", 2);
	exit(num);
}
