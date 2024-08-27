/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_lst.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 22:01:29 by hyunghki          #+#    #+#             */
/*   Updated: 2023/12/14 14:28:41 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include "lst.h"
#include "str.h"
#include "parse.h"

int	ft_lst_find(t_lst *s, char c)
{
	while (s != NULL && *s->data != '\0')
	{
		if (*s->data == c)
			return (E_TRUE);
		s = s->nxt;
	}
	return (E_FALSE);
}

t_lst	*ft_lst_trim(t_lst *lst)
{
	t_lst	*to_del;
	t_lst	*tmp;

	while (lst != NULL && ft_str_find(" \t\n", *lst->data) != 3)
		lst = lst->nxt;
	tmp = lst_back(lst);
	while (tmp != lst)
	{
		if (tmp == NULL || ft_str_find(" \t\n", *tmp->data) == 3)
			break ;
		to_del = tmp;
		tmp = tmp->prev;
		lst_delete(to_del);
	}
	return (lst);
}

char	*ft_c_str(t_lst *s, size_t size)
{
	char	*ret;
	size_t	i;
	int		quote_flag;

	quote_flag = 0;
	ret = ft_calloc(size + 1);
	i = 0;
	while (s != NULL && i < size)
	{
		if (*s->data == '\'' && s->info == E_T_ORIGIN && quote_flag != E_DQUOTE)
			quote_flag ^= E_SQUOTE;
		else if (*s->data == '\"' && s->info == E_T_ORIGIN && \
				quote_flag != E_SQUOTE)
			quote_flag ^= E_DQUOTE;
		else
			ret[i++] = *s->data;
		s = s->nxt;
	}
	return (ret);
}

t_lst	*mk_str_lst(char *s)
{
	size_t	i;
	t_lst	*ret;
	t_lst	*to_push;

	i = 0;
	ret = NULL;
	if (s == NULL)
		return (NULL);
	while (s[i])
	{
		to_push = mk_str_node(s + i, 0);
		lst_push(&ret, to_push);
		i++;
	}
	return (ret);
}

t_lst	*mk_str_node(char *s, int info)
{
	t_lst	*ret;
	char	*tmp;

	tmp = ft_substr(s, ft_strlen(s));
	ret = mk_lst(tmp, info);
	return (ret);
}
