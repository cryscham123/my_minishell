/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 17:28:20 by hyunghki          #+#    #+#             */
/*   Updated: 2023/12/14 19:30:45 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include "str.h"

extern int	g_status;

void	ft_putstr_fd(char *s, int fd)
{
	size_t	n;

	if (s == NULL)
		return ;
	n = ft_strlen(s);
	if (write(fd, s, n) < 0)
		exit(E_EXIT_FAILURE);
}

int	ft_error(char *msg, char *hint, int exit_status)
{
	char	*tmp_1;
	char	*tmp_2;
	char	*total_1;
	char	*total_2;

	total_1 = ft_append("\e[91mError: \e[0m", msg, '\0');
	if (hint != NULL)
	{
		tmp_1 = ft_append("\'", hint, '\0');
		tmp_2 = ft_append(tmp_1, "\'\n", '\0');
		total_2 = ft_append(total_1, tmp_2, '\0');
		free(tmp_1);
		free(tmp_2);
	}
	else
		total_2 = ft_append(total_1, "\n", '\0');
	ft_putstr_fd(total_2, 2);
	free(total_1);
	free(total_2);
	g_status = exit_status;
	return (exit_status);
}
