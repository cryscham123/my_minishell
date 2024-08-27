/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wild_card.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 22:01:29 by hyunghki          #+#    #+#             */
/*   Updated: 2023/12/14 15:21:51 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"
#include "utils.h"
#include "lst.h"
#include "str.h"

static int	wild_pattern_chk(t_lst *pattern, char *s, int flag, size_t size)
{
	char	c;

	c = '\0';
	if (size != 0 && pattern != NULL)
		c = *pattern->data;
	if (c == '\0' && *s == '\0')
		return (E_TRUE);
	if (c == '\'' && pattern->info == E_T_ORIGIN && flag != E_DQUOTE)
		return (wild_pattern_chk(pattern->nxt, s, flag ^ E_SQUOTE, size - 1));
	if (c == '\"' && pattern->info == E_T_ORIGIN && flag != E_SQUOTE)
		return (wild_pattern_chk(pattern->nxt, s, flag ^ E_DQUOTE, size - 1));
	if (flag == 0 && c == '*' && size != 1 && *s == '\0')
		return (wild_pattern_chk(pattern->nxt, s, flag, size - 1));
	if (flag == 0 && c == '*')
	{
		if (wild_pattern_chk(pattern->nxt, s, flag, size - 1) == E_TRUE)
			return (E_TRUE);
		if (wild_pattern_chk(pattern, s + 1, flag, size) == E_TRUE)
			return (E_TRUE);
	}
	if (c == *s)
		return (wild_pattern_chk(pattern->nxt, s + 1, flag, size - 1));
	return (E_FALSE);
}

static t_bool	is_wild_pattern(t_lst *pattern, size_t size, char *s)
{
	if (pattern == NULL || ft_lst_find(pattern, '*') == E_FALSE)
		return (E_FALSE);
	if (*s == '.' && *pattern->data != '.')
		return (E_FALSE);
	return (wild_pattern_chk(pattern, s, 0, size));
}

t_lst	*ft_wildcard(t_lst *to_find, size_t size)
{
	struct dirent	*item;
	DIR				*dp;
	t_lst			*ret;
	t_lst			*to_push;

	ret = NULL;
	dp = opendir("./");
	while (dp != NULL)
	{
		item = readdir(dp);
		if (item == NULL)
			break ;
		if (is_wild_pattern(to_find, size, item->d_name) != E_TRUE)
			continue ;
		to_push = mk_str_node(item->d_name, 0);
		lst_push(&ret, to_push);
	}
	closedir(dp);
	if (ret == NULL)
		ret = mk_lst(ft_c_str(to_find, size), 0);
	return (ret);
}
