#include "parse.h"
#include "cmd.h"
#include "lst.h"
#include "str.h"
#include "utils.h"

t_cmd	*ft_parse(t_env *env)
{
	char	*line;
	t_cmd	*ret;

	ft_signal(sigint_handler, SIG_IGN, 0);
	line = readline("\e[95mminishell$\e[0m \e[s");
	if (line == NULL)
	{
		ft_putstr_fd("\e[u\e[B\e[Aexit\n", 2);
		exit(E_EXIT_SUCCESS);
	}
	ret = ft_parse_line(line, env, E_TRUE);
	free(line);
	return (ret);
}

t_cmd	*ft_parse_line(char *line, t_env *env, t_bool save_history)
{
	char	*tmp;
	t_lex	*tokens;
	t_lex	*stream;
	t_cmd	*ret;

	ret = NULL;
	if (line != NULL && *line)
	{
		if (save_history == E_TRUE)
		add_history(line);
		tmp = ft_trim(line);
		if (*tmp != '\0')
		{
			tokens = lexer_tokenize(tmp);
			if (tokens != NULL)
			{
				stream = tokens;
				ret = parser_build_ast(&stream, 0, env);
				lexer_free(tokens);
			}
		}
	}
	return (ret);
}
