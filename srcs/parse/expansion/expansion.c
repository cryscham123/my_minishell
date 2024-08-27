/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 22:01:29 by hyunghki          #+#    #+#             */
/*   Updated: 2023/12/15 20:13:10 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"
#include "utils.h"
#include "lst.h"
#include "str.h"

static t_parse	get_total_ret(t_lst **ret, t_lst **s)
{
	t_lst	*tmp;
	int		wi;
	int		quote_flag;
	size_t	size;

	quote_flag = 0;
	size = 0;
	tmp = *s;
	while ((*s) != NULL)
	{
		wi = (*s)->info;
		if (*(*s)->data == '\'' && wi == E_T_ORIGIN && quote_flag != E_DQUOTE)
			quote_flag ^= E_SQUOTE;
		if (*(*s)->data == '\"' && wi == E_T_ORIGIN && quote_flag != E_SQUOTE)
			quote_flag ^= E_DQUOTE;
		if (quote_flag == 0 && ft_str_find(" \t\n", *(*s)->data) != 3)
			break ;
		size++;
		(*s) = (*s)->nxt;
	}
	lst_push(ret, ft_wildcard(tmp, size));
	if ((*s) == NULL)
		return (E_PARSE_SUCCESS);
	return (E_PARSE_PROGRESS);
}

static t_lst	*ft_word_split(t_lst *s)
{
	t_parse	flag;
	t_lst	*ret;
	t_lst	*to_push;

	flag = E_PARSE_PROGRESS;
	ret = NULL;
	while (flag == E_PARSE_PROGRESS)
	{
		to_push = NULL;
		while (s != NULL && ft_str_find(" \t\n", *s->data) != 3)
			s = s->nxt;
		if (s == NULL)
			flag = E_PARSE_SUCCESS;
		else
			flag = get_total_ret(&to_push, &s);
		lst_push(&ret, to_push);
	}
	return (ret);
}

static t_lst	*find_target(t_lst **word_lst, t_lst *ev, int quote_flag)
{
	t_lst	*tmp;
	char	*target_val;
	size_t	size;

	(*word_lst) = (*word_lst)->nxt;
	if (quote_flag == 0 && *word_lst != NULL && \
		(*(*word_lst)->data == '\'' || *(*word_lst)->data == '\"'))
		return (NULL);
	tmp = *word_lst;
	size = (*word_lst != NULL && *(*word_lst)->data == '?');
	while (*word_lst != NULL)
	{
		if (ft_str_find(" \t\n\'\"|&><$=*?", *(*word_lst)->data) != 13)
			break ;
		(*word_lst) = (*word_lst)->nxt;
		size++;
	}
	if (size == 1 && *word_lst != NULL && *(*word_lst)->data == '?')
		(*word_lst) = (*word_lst)->nxt;
	target_val = ft_c_str(tmp, size);
	tmp = ft_env_find(ev, target_val);
	free(target_val);
	if (tmp == NULL)
		return (NULL);
	return (mk_str_lst(tmp->data + tmp->info + 1));
}

static t_lst	*ft_trans(t_lst *word_lst, t_lst *ev)
{
	int		quote_flag;
	t_lst	*ret;

	ret = NULL;
	quote_flag = 0;
	while (word_lst != NULL)
	{
		if (*word_lst->data == '\'' && quote_flag != E_DQUOTE)
			quote_flag ^= E_SQUOTE;
		if (*word_lst->data == '\"' && quote_flag != E_SQUOTE)
			quote_flag ^= E_DQUOTE;
		if (quote_flag != E_SQUOTE && *word_lst->data == '$')
			lst_push(&ret, find_target(&word_lst, ev, quote_flag));
		else
		{
			lst_push(&ret, mk_str_node(word_lst->data, E_T_ORIGIN));
			word_lst = word_lst->nxt;
		}
	}
	return (ret);
}

t_lst	*ft_expansion(char *word, t_lst *ev)
{
	t_lst	*word_lst;
	t_lst	*translated;
	t_lst	*ret;

	word_lst = mk_str_lst(word);
	translated = ft_trans(word_lst, ev);
	ft_lst_free(word_lst);
	ret = ft_word_split(ft_lst_trim(translated));
	ft_lst_free(translated);
	return (ret);
}
