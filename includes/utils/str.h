/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 12:55:27 by hyunghki          #+#    #+#             */
/*   Updated: 2023/12/13 15:28:24 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STR_H
# define STR_H

# include "utils.h"
# include "lst.h"

typedef enum e_str_info
{
	E_DEL = 128,
	E_NO_TRANS = 256,
	E_NO_PARSE = 512
}	t_str_info;

int		ft_strncmp(char *s1, char *s2, size_t n);
int		ft_strcmp(char *s1, char *s2);
size_t	ft_str_find(char *s, char to_find);
size_t	ft_strlen(char *s);
char	*ft_strdup(char *s);
char	*ft_substr(char *s, size_t n);
void	ft_strncpy(char *dst, char *src, size_t n);
t_bool	ft_str_is_num(char *s);
char	*ft_str_plus(char *s_1, char *s_2, size_t s1_size, size_t s2_size);
void	ft_str_reverse(char *s);
char	*ft_trim(char *s);
char	*ft_itoa(int a, int cnt);
char	**mk_argv(t_lst *lst);
char	*ft_append(char *s1, char *s2, char middle);
char	*delete_quote(char *s, t_bool *info);

#endif