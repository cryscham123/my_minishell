#include "parse.h"

t_bool	quote_update(int *quote_flag, char c)
{
	switch (c)
	{
		case '\'':
			if (*quote_flag != E_DQUOTE)
			{
				*quote_flag ^= E_SQUOTE;
				return (E_TRUE);
			}
			break ;
		case '\"':
			if (*quote_flag != E_SQUOTE)
			{
				*quote_flag ^= E_DQUOTE;
				return (E_TRUE);
			}
			break ;
		default:
			break ;
	}
	return (E_FALSE);
}
