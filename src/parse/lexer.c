#include "parse.h"
#include "utils.h"
#include "str.h"

static t_lex	*mk_token(t_token type, char *word)
{
	t_lex	*token;

	token = ft_zalloc(sizeof(t_lex));
	token->type = type;
	token->word = word;
	return (token);
}

static void	token_push(t_lex **tokens, t_lex *to_push)
{
	t_lex	*tmp;

	if (*tokens == NULL)
	{
		*tokens = to_push;
		return ;
	}
	tmp = *tokens;
	while (tmp->nxt != NULL)
		tmp = tmp->nxt;
	tmp->nxt = to_push;
}

void	lexer_free(t_lex *tokens)
{
	t_lex	*tmp;

	while (tokens != NULL)
	{
		tmp = tokens;
		tokens = tokens->nxt;
		free(tmp->word);
		free(tmp);
	}
}

static void	skip_spaces(char **line)
{
	while (**line == ' ' || **line == '\t' || **line == '\n')
		(*line)++;
}

static t_bool	is_word_delimiter(char c)
{
	if (c == '\0' || c == ' ' || c == '\t' || c == '\n')
		return (E_TRUE);
	if (c == '(' || c == ')' || c == '|' || c == '&')
		return (E_TRUE);
	if (c == '>' || c == '<')
		return (E_TRUE);
	return (E_FALSE);
}

static t_lex	*lex_word(char **line)
{
	char	*start;
	int		quote_flag;
	size_t	size;

	start = *line;
	quote_flag = 0;
	size = 0;
	while ((*line)[size] != '\0')
	{
		quote_update(&quote_flag, (*line)[size]);
		if (quote_flag == 0 && is_word_delimiter((*line)[size]) == E_TRUE)
			break ;
		size++;
	}
	if (quote_flag != 0)
	{
		ft_syntax_error(ft_substr(start, size), quote_flag);
		return (NULL);
	}
	(*line) += size;
	return (mk_token(E_T_WORD, ft_substr(start, size)));
}

static t_lex	*lex_meta(char **line)
{
	t_token	type;
	size_t	size;

	size = 1;
	switch (**line)
	{
		case '(':
			type = E_T_LPAREN;
			break ;
		case ')':
			type = E_T_RPAREN;
			break ;
		case '|':
			type = E_T_PIPE;
			if (*(*line + 1) == '|')
				type = E_T_OR;
			break ;
		case '&':
			type = E_T_AND;
			break ;
		case '>':
			type = E_T_OUTPUT;
			if (*(*line + 1) == '>')
				type = E_T_APPEND;
			break ;
		default:
			type = E_T_INPUT;
			if (*(*line + 1) == '<')
				type = E_T_HEREDOC;
			break ;
	}
	if (type == E_T_OR || type == E_T_AND || \
		type == E_T_APPEND || type == E_T_HEREDOC)
		size = 2;
	(*line) += size;
	return (mk_token(type, ft_substr(*line - size, size)));
}

t_lex	*lexer_tokenize(char *line)
{
	t_lex	*tokens;
	t_lex	*token;

	tokens = NULL;
	while (*line != '\0')
	{
		skip_spaces(&line);
		if (*line == '\0')
			break ;
		if (*line == '&' && *(line + 1) != '&')
		{
			lexer_free(tokens);
			ft_error(F_ERROR_NOT_SUPPORT, NULL, E_PARSE_ERROR);
			return (NULL);
		}
		if (is_word_delimiter(*line) == E_TRUE)
			token = lex_meta(&line);
		else
			token = lex_word(&line);
		if (token == NULL)
		{
			lexer_free(tokens);
			return (NULL);
		}
		token_push(&tokens, token);
	}
	return (tokens);
}
