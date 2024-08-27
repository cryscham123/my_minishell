/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 02:16:31 by hyunghki          #+#    #+#             */
/*   Updated: 2023/12/13 18:58:44 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"
#include "cmd.h"
#include "lst.h"
#include "str.h"
#include "utils.h"

t_cmd	*ft_parse(t_lst *ev)
{
	char	*line;
	char	*tmp;
	t_cmd	*ret;

	ft_signal(sigint_handler, SIG_IGN, 0);
	ret = NULL;
	line = readline("\e[95mminishell$\e[0m \e[s");
	if (line == NULL)
	{
		ft_putstr_fd("\e[u\e[B\e[Aexit\n", 2);
		exit(E_EXIT_SUCCESS);
	}
	if (*line)
	{
		add_history(line);
		tmp = ft_trim(line);
		if (*tmp != '\0')
			ret = ft_split(&tmp, 0, ev);
	}
	free(line);
	return (ret);
}
