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

static t_bool	is_command_complete(t_token tf)
{
	return (tf == E_T_WORD || tf == E_T_SUB);
}

int	ft_append_cmd_word(t_cmd **ret, char *word, t_token tf, t_env *env)
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
		to_push = ft_heredoc(word, env);
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

static char	*meta_error_word(t_lex *token)
{
	return (ft_strdup(token->word));
}

static int	parse_pipe_meta(t_lex *token, t_token *tf)
{
	if (is_command_complete(*tf) == E_FALSE)
		return (ft_syntax_error(meta_error_word(token), *tf));
	*tf = token->type;
	return (E_PARSE_PROGRESS);
}

static int	parse_and_meta(t_lex *token, t_token *tf)
{
	if (is_command_complete(*tf) == E_FALSE)
		return (ft_syntax_error(meta_error_word(token), *tf));
	*tf = E_T_AND;
	return (E_PARSE_PROGRESS);
}

static int	parse_output_meta(t_lex *token, t_token *tf)
{
	if (is_redir_flag(*tf))
		return (ft_syntax_error(meta_error_word(token), *tf));
	*tf = token->type;
	return (E_PARSE_PROGRESS);
}

static int	parse_input_meta(t_lex *token, t_token *tf)
{
	if (is_redir_flag(*tf))
		return (ft_syntax_error(meta_error_word(token), *tf));
	*tf = token->type;
	return (E_PARSE_PROGRESS);
}

int	parse_meta(t_lex **tokens, t_token *tf, t_cmd **ret)
{
	int	flag;

	flag = E_PARSE_PROGRESS;
	switch ((*tokens)->type)
	{
		case E_T_PIPE:
		case E_T_OR:
			flag = parse_pipe_meta(*tokens, tf);
			break ;
		case E_T_AND:
			flag = parse_and_meta(*tokens, tf);
			break ;
		case E_T_OUTPUT:
		case E_T_APPEND:
			flag = parse_output_meta(*tokens, tf);
			break ;
		case E_T_INPUT:
		case E_T_HEREDOC:
			flag = parse_input_meta(*tokens, tf);
			break ;
		default:
			flag = ft_syntax_error(meta_error_word(*tokens), *tf);
			break ;
	}
	if (flag != E_PARSE_PROGRESS)
		return (flag);
	*tokens = (*tokens)->nxt;
	ft_cmd_context_switch(ret, *tf);
	return (E_PARSE_PROGRESS);
}
