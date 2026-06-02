#ifndef PARSE_H
# define PARSE_H

# include "cmd.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>

typedef enum e_token
{
	E_T_BEGIN = 3,
	E_T_WORD,
	E_T_PIPE,
	E_T_AND,
	E_T_OR,
	E_T_INPUT,
	E_T_OUTPUT,
	E_T_HEREDOC,
	E_T_APPEND,
	E_T_LPAREN,
	E_T_RPAREN,
	E_T_SUB,
	E_T_ORIGIN
}	t_token;

typedef enum e_parse
{
	E_PARSE_SUCCESS,
	E_PARSE_PROGRESS,
	E_PARSE_ERROR,
	E_PARSE_SUB_PROGRESS,
}	t_parse;

typedef enum e_quote
{
	E_SQUOTE = 1,
	E_DQUOTE = 2
}	t_quote;

typedef struct s_lex
{
	t_token			type;
	char			*word;
	struct s_lex	*nxt;
}	t_lex;

t_cmd	*ft_parse(t_env *env);
t_cmd	*ft_parse_line(char *line, t_env *env, t_bool save_history);
t_bool	quote_update(int *quote_flag, char c);
t_lex	*lexer_tokenize(char *line);
void	lexer_free(t_lex *tokens);
t_cmd	*parser_build_ast(t_lex **tokens, size_t depth, t_env *env);
void	pipe_cmd_push(t_cmd *ret);
int		parse_meta(t_lex **tokens, t_token *tf, t_cmd **ret);
int		ft_append_cmd_word(t_cmd **ret, char *word, t_token tf, t_env *env);
t_parse	ft_syntax_error(char *error_token, int tf);
t_parse	ft_sub_split(t_lex **tokens, size_t depth, t_cmd **ret, t_env *env);
t_bool	is_redir_flag(t_token tf);
t_lst	*ft_expansion(char *word, t_env *env);
t_lst	*ft_wildcard(t_lst *to_find, size_t size);

#endif
