#include "builtin.h"
#include "utils.h"
#include "str.h"

int	ft_echo(char **av)
{
	t_bool	is_new_line;

	is_new_line = E_TRUE;
	while (*av != NULL && echo_option_chk(*av) == E_TRUE)
	{
		is_new_line = E_FALSE;
		av++;
	}
	while (*av != NULL)
	{
		ft_putstr_fd(*av, 1);
		av++;
		if (*av != NULL)
			ft_putstr_fd(" ", 1);
	}
	if (is_new_line == E_TRUE)
		ft_putstr_fd("\n", 1);
	return (E_BUILT_IN_SUCCESS);
}

int	ft_cd(char **av, t_env *env)
{
	int		flag;

	if (*av == NULL || **av == '\0')
		return (E_BUILT_IN_SUCCESS);
	flag = ft_av_validate_dir(av);
	if (flag != E_BUILT_IN_SUCCESS)
		return (flag);
	flag = ft_set_env_pwd("OLDPWD", env);
	chdir(*av);
	if (flag == E_BUILT_IN_SUCCESS)
		flag = ft_set_env_pwd("PWD", env);
	return (flag);
}

int	ft_pwd(void)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (pwd == NULL)
		return (ft_error(F_ERROR_PWD, NULL, E_PWD_NOT_EXIST));
	ft_putstr_fd(pwd, 1);
	ft_putstr_fd("\n", 1);
	free(pwd);
	return (E_BUILT_IN_SUCCESS);
}

int	ft_export(char **av, t_env *env)
{
	int		flag;

	flag = 0;
	while (*av != NULL)
	{
		flag = ft_av_validate_ev(*av);
		switch (flag)
		{
			case E_BUILT_IN_SUCCESS:
				ft_unset_unit(*av, env, flag);
				env_set_entry(env, *av);
				break ;
			case E_EV_NO_VALUE:
				flag = E_BUILT_IN_SUCCESS;
				break ;
			case E_EV_KEY_ERROR:
				flag = E_BUILT_IN_FAIL;
				break ;
			default:
				break ;
		}
		av++;
	}
	return (flag);
}

int	ft_unset(char **av, t_env *env)
{
	int	flag;

	flag = 0;
	while (*av != NULL)
	{
		flag = ft_av_validate_ev(*av);
		if (flag == E_EV_NO_VALUE)
		{
			ft_unset_unit(*av, env, flag);
			flag = E_BUILT_IN_SUCCESS;
		}
		else
			flag = E_BUILT_IN_FAIL;
		av++;
	}
	return (flag);
}
