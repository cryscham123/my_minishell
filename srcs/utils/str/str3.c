/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 13:12:43 by hyunghki          #+#    #+#             */
/*   Updated: 2023/12/13 17:37:04 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include "str.h"
#include "parse.h"

char	*ft_strdup(char *s)
{
	return (ft_substr(s, ft_strlen(s)));
}

char	*delete_quote(char *s, t_bool *info)
{
	char	*ret;
	int		i;
	t_quote	quote_flag;

	i = 0;
	quote_flag = 0;
	ret = ft_calloc(ft_strlen(s) + 1);
	while (*s != '\0')
	{
		if (*s == '\'' && quote_flag != E_DQUOTE)
			quote_flag ^= E_SQUOTE;
		else if (*s == '\"' && quote_flag != E_SQUOTE)
			quote_flag ^= E_DQUOTE;
		else
			ret[i++] = *s;
		if (quote_flag != 0)
			(*info) = E_FALSE;
		s++;
	}
	return (ret);
}

t_bool	ft_str_is_num(char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (E_FALSE);
		i++;
	}
	return (E_TRUE);
}

void	ft_str_reverse(char *s)
{
	char	tmp;
	size_t	i;
	size_t	n;

	i = 0;
	n = ft_strlen(s);
	if (n == 0)
		return ;
	while (i < n - 1 - i)
	{
		tmp = s[i];
		s[i] = s[n - 1 - i];
		s[n - 1 - i] = tmp;
		i++;
	}
}

char	*ft_str_plus(char *s_1, char *s_2, size_t s1_size, size_t s2_size)
{
	char	*ret;
	size_t	i;
	size_t	n;
	size_t	tmp;

	n = (s1_size > s2_size) * s1_size + (s1_size <= s2_size) * s2_size;
	ret = ft_calloc(n + 2);
	i = 0;
	tmp = 0;
	ft_str_reverse(s_1);
	ft_str_reverse(s_2);
	while (i < n || tmp != 0)
	{
		if (i < s1_size)
			tmp += s_1[i] - '0';
		if (i < s2_size)
			tmp += s_2[i] - '0';
		ret[i] = tmp % 10 + '0';
		tmp /= 10;
		i++;
	}
	ft_str_reverse(ret);
	return (ret);
}
