/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 15:36:55 by hyunghki          #+#    #+#             */
/*   Updated: 2023/12/15 18:59:31 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"
#include "cmd.h"
#include "utils.h"
#include "files.h"
#include "str.h"

static t_cmd	*find_cmd_to_push_word(t_cmd *target)
{
	t_cmd	*nxt;

	while (target != NULL)
	{
		if (target->cmd_info == E_T_SUB || target->cmd_info == 0)
			return (target);
		nxt = nxt_cmd(target);
		if (nxt == NULL && target->cmd_info == E_T_PIPE)
			return (target->cmd->pb_cmd.target);
		target = nxt;
	}
	return (target);
}

static void	ft_cmd_context_switch(t_cmd **ret, t_token tf)
{
	t_cmd	*new_cmd;

	if (is_redir_flag(tf))
		return ;
	if (tf == E_T_PIPE)
		pipe_cmd_push(*ret);
	else
	{
		new_cmd = mk_bi_cmd(*ret, NULL, tf);
		*ret = new_cmd;
	}
}

t_bool	is_redir_flag(t_token tf)
{
	return (tf == E_T_INPUT || \
			tf == E_T_OUTPUT || \
			tf == E_T_HEREDOC || \
			tf == E_T_APPEND);
}

int	ft_append_cmd_word(t_cmd **ret, char *word, t_token tf, t_lst *ev)
{
	t_cmd	*target;
	t_lst	*to_push;

	target = find_cmd_to_push_word(*ret);
	if (target == NULL)
	{
		target = mk_pu_cmd();
		cmd_push(ret, target);
	}
	if (tf == E_T_HEREDOC)
		to_push = ft_heredoc(word, ev);
	else
		to_push = mk_lst(word, tf);
	if (to_push == NULL && tf == E_T_HEREDOC)
		return (E_PARSE_ERROR);
	if (is_redir_flag(tf))
		lst_push(&target->cmd->pu_cmd.redir, to_push);
	else
		lst_push(&target->cmd->pu_cmd.argv, to_push);
	return (E_PARSE_PROGRESS);
}

int	parse_meta(char **s, t_token *tf, t_cmd **ret)
{
	if (**s == '|' && *(*s + 1) == '|' && \
		(*tf == E_T_WORD || *tf == E_T_SUB))
		*tf = E_T_OR;
	else if (**s == '|' && \
		(*tf == E_T_WORD || *tf == E_T_SUB))
		*tf = E_T_PIPE;
	else if (**s == '&' && *(*s + 1) == '&' && \
		(*tf == E_T_WORD || *tf == E_T_SUB))
		*tf = E_T_AND;
	else if (**s == '&' && *(*s + 1) != '&')
		return (ft_error(F_ERROR_NOT_SUPPORT, NULL, E_PARSE_ERROR));
	else if (**s == '>' && *(*s + 1) == '>' && !is_redir_flag(*tf))
		*tf = E_T_APPEND;
	else if (**s == '>' && !is_redir_flag(*tf))
		*tf = E_T_OUTPUT;
	else if (**s == '<' && *(*s + 1) == '<' && !is_redir_flag(*tf))
		*tf = E_T_HEREDOC;
	else if (**s == '<' && !is_redir_flag(*tf))
		*tf = E_T_INPUT;
	else
		return (ft_syntax_error(ft_substr(*s, 1 + (**s == *(*s + 1))), *tf));
	(*s) += 1 + (**s == *(*s + 1));
	ft_cmd_context_switch(ret, *tf);
	return (E_PARSE_PROGRESS);
}
