#include "parse.h"
#include "cmd.h"
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

static char	*token_error_word(t_lex *token)
{
	if (token == NULL)
		return (ft_strdup(""));
	return (ft_strdup(token->word));
}

static int	chk_split_begin(t_lex **tokens, t_token *tf)
{
	if (*tf != E_T_BEGIN && \
		*tf != E_T_PIPE && \
		*tf != E_T_AND && \
		*tf != E_T_OR)
		return (ft_syntax_error(token_error_word(*tokens), *tf));
	*tokens = (*tokens)->nxt;
	*tf = E_T_SUB;
	return (E_PARSE_SUB_PROGRESS);
}

static int	chk_split_end(t_lex **tokens, t_token tf, size_t depth)
{
	if (*tokens == NULL && depth == 0 && \
		(tf == E_T_WORD || tf == E_T_SUB))
		return (E_PARSE_SUCCESS);
	if (*tokens != NULL && (*tokens)->type == E_T_RPAREN && depth > 0 && \
		(tf == E_T_WORD || tf == E_T_SUB))
	{
		*tokens = (*tokens)->nxt;
		return (E_PARSE_SUCCESS);
	}
	return (ft_syntax_error(token_error_word(*tokens), tf));
}

static int	parse_word(t_lex **tokens, t_token *tf, t_cmd **ret, t_env *env)
{
	char	*word;
	int		flag;

	word = ft_strdup((*tokens)->word);
	if (*tf == E_T_SUB)
		return (ft_syntax_error(word, *tf));
	flag = ft_append_cmd_word(ret, word, *tf, env);
	if (flag == E_PARSE_PROGRESS)
	{
		*tf = E_T_WORD;
		*tokens = (*tokens)->nxt;
	}
	return (flag);
}

static t_parse	parse_next(t_lex **tokens, t_token *tf, t_cmd **ret, t_env *env)
{
	switch ((*tokens)->type)
	{
		case E_T_PIPE:
		case E_T_AND:
		case E_T_OR:
		case E_T_OUTPUT:
		case E_T_APPEND:
		case E_T_INPUT:
		case E_T_HEREDOC:
			return (parse_meta(tokens, tf, ret));
		case E_T_LPAREN:
			return (chk_split_begin(tokens, tf));
		case E_T_WORD:
			return (parse_word(tokens, tf, ret, env));
		default:
			return (ft_syntax_error(token_error_word(*tokens), *tf));
	}
}

t_cmd	*parser_build_ast(t_lex **tokens, size_t depth, t_env *env)
{
	t_token	tf;
	t_parse	flag;
	t_cmd	*ret;

	tf = E_T_BEGIN;
	flag = E_PARSE_PROGRESS;
	ret = NULL;
	while (flag == E_PARSE_PROGRESS)
	{
		if (*tokens == NULL || (*tokens)->type == E_T_RPAREN)
			flag = chk_split_end(tokens, tf, depth);
		else
			flag = parse_next(tokens, &tf, &ret, env);
		if (flag == E_PARSE_SUB_PROGRESS)
			flag = ft_sub_split(tokens, depth + 1, &ret, env);
	}
	if (flag == E_PARSE_ERROR)
		return (cmd_free(ret));
	return (ret);
}
