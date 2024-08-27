/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_free.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 13:11:05 by hyunghki          #+#    #+#             */
/*   Updated: 2023/12/12 17:39:06 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lst.h"
#include "utils.h"
#include "parse.h"
#include "files.h"

void	lst_delete(t_lst *to_del)
{
	if (to_del == NULL)
		return ;
	if (to_del->prev != NULL)
		to_del->prev->nxt = to_del->nxt;
	if (to_del->nxt != NULL)
		to_del->nxt->prev = to_del->prev;
	to_del->nxt = NULL;
	ft_lst_free(to_del);
}

void	*ft_node_free(char *data, int info)
{
	if (data == NULL)
		return (NULL);
	if (info == E_T_HEREDOC)
		unlink((char *)data);
	free(data);
	return (NULL);
}

void	*ft_lst_free(t_lst *lst)
{
	t_lst	*tmp;

	while (lst != NULL)
	{
		ft_node_free(lst->data, lst->info);
		tmp = lst;
		lst = lst->nxt;
		free(tmp);
	}
	return (NULL);
}
