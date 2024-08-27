/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 17:28:20 by hyunghki          #+#    #+#             */
/*   Updated: 2023/12/13 16:14:18 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include "str.h"
#include "lst.h"

char	*ft_trim(char *s)
{
	int	i;

	while (*s && (*s == ' ' || *s == '\t' || *s == '\n'))
		s++;
	i = ft_strlen(s) - 1;
	while (i >= 0)
	{
		if (s[i] != ' ' && s[i] != '\t')
			break ;
		s[i] = '\0';
		i--;
	}
	return (s);
}

char	*ft_append(char *s1, char *s2, char middle)
{
	char	*target;
	int		s1_len;
	int		s2_len;
	int		i;
	int		j;

	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	target = ft_calloc(s1_len + s2_len + (middle != 0) + 1);
	if (target == NULL)
		return (NULL);
	i = -1;
	while (++i < s1_len)
		target[i] = s1[i];
	if (middle != 0)
		target[i++] = middle;
	j = -1;
	while (++j < s2_len)
		target[i + j] = s2[j];
	return (target);
}

char	*ft_itoa(int a, int cnt)
{
	char	*target;

	if (a < 10)
	{
		target = ft_calloc(cnt + 1);
		if (target == NULL)
			return (NULL);
		target[0] = "0123456789"[a % 10];
		return (target);
	}
	target = ft_itoa(a / 10, cnt + 1);
	if (target == NULL)
		return (NULL);
	target[ft_strlen(target)] = "0123456789"[a % 10];
	return (target);
}

int	ft_strcmp(char *s1, char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] && s2[i])
	{
		if (s1[i] != s2[i])
			return (s1[i] - s2[i]);
		i++;
	}
	return (s1[i] - s2[i]);
}

char	**mk_argv(t_lst *lst)
{
	char	**target;
	int		size;
	int		i;

	size = ft_lst_size(lst);
	target = ft_calloc(sizeof(char *) * (size + 1));
	i = 0;
	while (lst != NULL)
	{
		target[i] = ft_substr(lst->data, ft_strlen(lst->data));
		if (target[i] == NULL)
			return (ft_argv_free(target));
		i++;
		lst = lst->nxt;
	}
	return (target);
}
