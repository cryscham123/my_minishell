/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 04:28:02 by hyunghki          #+#    #+#             */
/*   Updated: 2023/12/16 13:37:02 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILT_IN_H
# define BUILT_IN_H

# include "cmd.h"
# include "utils.h"

typedef enum e_built_in_status
{
	E_BUILT_IN_NO_BUILT = -5,
	E_PWD_NOT_EXIST = -1,
	E_EV_NO_VALUE = -2,
	E_EV_KEY_ERROR = -3,
	E_BUILT_IN_FAIL = -1,
	E_BUILT_IN_SUCCESS = 0,
	E_BUILT_IN_EXIT_FAIL = 255,
}	t_built_in_status;

int		ft_echo(char **av);
int		ft_cd(char **av, t_lst *ev);
int		ft_pwd(void);
int		ft_export(char **av, t_lst *ev);
int		ft_unset(char **av, t_lst *ev);
int		ft_env(t_lst *ev);
int		ft_exit(char **av, t_bool is_forked);
t_bool	echo_option_chk(char *s);
int		ft_av_validate_dir(char **s);
int		ft_set_ev_pwd(char *key, t_lst *ev);
int		ft_av_validate_ev(char *s);
void	ft_unset_unit(char *s, t_lst *ev, int flag);
int		ft_av_exit(char *s, t_bool is_forked);

#endif
