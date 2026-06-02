#include "builtin.h"
#include "utils.h"

int	ft_env(t_env *env)
{
	env_print(env);
	return (0);
}

int	ft_exit(char **av, t_bool is_forked)
{
	int		num;

	num = ft_shell_status();
	if (*av != NULL)
	{
		num = ft_av_exit(*av, is_forked);
		if (*(av + 1) != NULL)
			return (ft_error(F_ERROR_ARG, NULL, E_BUILT_IN_FAIL));
	}
	if (is_forked == E_FALSE)
		ft_putstr_fd("exit\n", 2);
	exit(num);
}
