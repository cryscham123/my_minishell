/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 11:15:25 by hyunghki          #+#    #+#             */
/*   Updated: 2023/12/13 23:06:13 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include "lst.h"

t_lst	*mk_lst(char *data, int info)
{
	t_lst	*target;

	target = ft_calloc(sizeof(t_lst));
	target->data = data;
	target->info = info;
	return (target);
}

t_lst	*lst_back(t_lst *lst)
{
	if (lst == NULL)
		return (NULL);
	while (lst->nxt != NULL)
		lst = lst->nxt;
	return (lst);
}

size_t	ft_lst_size(t_lst *lst)
{
	size_t	ret;

	ret = 0;
	while (lst != NULL)
	{
		ret++;
		lst = lst->nxt;
	}
	return (ret);
}

void	lst_push(t_lst **target, t_lst *to_push)
{
	t_lst	*last_node;

	last_node = lst_back(*target);
	if (last_node == NULL)
	{
		*target = to_push;
		return ;
	}
	last_node->nxt = to_push;
	if (to_push != NULL)
		to_push->prev = last_node;
}
