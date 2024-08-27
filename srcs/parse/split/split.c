/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 07:05:22 by hyunghki          #+#    #+#             */
/*   Updated: 2023/12/15 14:05:54 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"
#include "cmd.h"
#include "lst.h"
#include "utils.h"
#include "str.h"

t_parse	ft_syntax_error(char *error_token, int tf)
{
	if (tf == E_SQUOTE || \
		tf == E_DQUOTE || \
		*error_token == '\0')
		ft_error(F_ERROR_UNCLOSE, NULL, E_PARSE_ERROR);
	else
		ft_error(F_ERROR_SYNTAX, error_token, E_PARSE_ERROR);
	free(error_token);
	return (E_PARSE_ERROR);
}

static int	chk_split_begin(char **s, t_token *tf)
{
	if (*tf != E_T_BEGIN && \
		*tf != E_T_PIPE && \
		*tf != E_T_AND && \
		*tf != E_T_OR)
		return (ft_syntax_error(ft_substr(*s, 1), *tf));
	(*s) += 1;
	(*tf) = E_T_SUB;
	return (E_PARSE_SUB_PROGRESS);
}

static int	chk_split_end(char **s, t_token tf, size_t depth)
{
	if (**s == '\0' && depth == 0 && (tf == E_T_WORD || tf == E_T_SUB))
		return (E_PARSE_SUCCESS);
	if (**s == ')' && depth > 0 && tf == E_T_WORD)
	{
		(*s) += 1;
		return (E_PARSE_SUCCESS);
	}
	return (ft_syntax_error(ft_substr(*s, 1), tf));
}

static int	parse_word(char **s, t_token *tf, t_cmd **ret, t_lst *ev)
{
	char	*word;
	int		flag;
	size_t	size;

	size = 0;
	flag = 0;
	while ((*s)[size] != '\0')
	{
		if ((*s)[size] == '\'' && flag != E_DQUOTE)
			flag ^= E_SQUOTE;
		if ((*s)[size] == '\"' && flag != E_SQUOTE)
			flag ^= E_DQUOTE;
		if (flag == 0 && ft_str_find(" \t\n()|&><", (*s)[size]) != 9)
			break ;
		size++;
	}
	word = ft_substr(*s, size);
	if (*tf == E_T_SUB)
		return (ft_syntax_error(word, *tf));
	if (flag != 0)
		return (ft_syntax_error(word, flag));
	flag = ft_append_cmd_word(ret, word, *tf, ev);
	*tf = E_T_WORD;
	(*s) += size;
	return (flag);
}

t_cmd	*ft_split(char **s, size_t depth, t_lst *ev)
{
	t_token		tf;
	t_parse		flag;
	t_cmd		*ret;

	tf = E_T_BEGIN;
	flag = E_PARSE_PROGRESS;
	ret = NULL;
	while (flag == E_PARSE_PROGRESS)
	{
		while (**s != '\0' && ft_str_find(" \t\n", **s) != 3)
			(*s) += 1;
		if (ft_str_find("|&><", **s) != 4)
			flag = parse_meta(s, &tf, &ret);
		else if (**s == '(')
			flag = chk_split_begin(s, &tf);
		else if (**s == ')' || **s == '\0')
			flag = chk_split_end(s, tf, depth);
		else
			flag = parse_word(s, &tf, &ret, ev);
		if (flag == E_PARSE_SUB_PROGRESS)
			flag = ft_sub_split(s, depth + 1, &ret, ev);
	}
	if (flag == E_PARSE_ERROR)
		return (cmd_free(ret));
	return (ret);
}
