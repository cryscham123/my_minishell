#include "builtin.h"
#include "files.h"
#include "utils.h"
#include "str.h"

t_bool	echo_option_chk(char *s)
{
	int	i;

	if (s == NULL)
		return (E_FALSE);
	i = 1;
	if (s[0] != '-' || s[1] == '\0')
		return (E_FALSE);
	while (s[i] == 'n')
		i++;
	return (s[i] == '\0');
}

int	ft_av_validate_dir(char **s)
{
	struct stat	buf;

	stat(*s, &buf);
	if (access(*s, F_OK) != 0)
		return (ft_error(F_ERROR_FILE, *s, E_BUILT_IN_FAIL));
	if (access(*s, X_OK) != 0)
		return (ft_error(F_ERROR_ACCESS, *s, E_BUILT_IN_FAIL));
	if (!S_ISDIR(buf.st_mode))
		return (ft_error(F_ERROR_NO_DIR, *s, E_BUILT_IN_FAIL));
	return (E_BUILT_IN_SUCCESS);
}

int	ft_set_env_pwd(char *key, t_env *env)
{
	char	*pwd;
	int		ret;

	pwd = getcwd(NULL, 0);
	if (pwd == NULL)
		return (ft_error(F_ERROR_FILE, NULL, E_BUILT_IN_FAIL));
	ret = env_set(env, key, pwd);
	free(pwd);
	return (ret);
}

int	ft_av_validate_ev(char *s)
{
	int	i;

	if (*s >= '0' && *s <= '9')
		return (ft_error(F_ERROR_KEY, s, E_EV_KEY_ERROR));
	i = 0;
	while (s[i] && s[i] != '=')
	{
		if (!(i != 0 && s[i] >= '0' && s[i] <= '9') \
			&& !(s[i] >= 'a' && s[i] <= 'z') \
			&& !(s[i] >= 'A' && s[i] <= 'Z') && s[i] != '_')
			return (ft_error(F_ERROR_KEY, s, E_EV_KEY_ERROR));
		i++;
	}
	if (i == 0)
		return (ft_error(F_ERROR_KEY, s, E_EV_KEY_ERROR));
	if (s[i] != '=')
		return (E_EV_NO_VALUE);
	return (0);
}

void	ft_unset_unit(char *s, t_env *env, int flag)
{
	int		i;

	i = 0;
	if (flag != E_EV_NO_VALUE)
	{
		i = ft_str_find(s, '=');
		s[i] = '\0';
	}
	env_unset(env, s);
	if (flag != E_EV_NO_VALUE)
		s[i] = '=';
}
