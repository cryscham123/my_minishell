#include "cmd.h"
#include "str.h"
#include "utils.h"
#include "files.h"
#include "builtin.h"

typedef int	(*t_builtin_runner)(char **argv, t_env *env, t_bool is_forked);

typedef struct s_builtin_dispatch
{
	char				*name;
	t_builtin_runner	run;
}	t_builtin_dispatch;

static int	run_echo(char **argv, t_env *env, t_bool is_forked)
{
	(void)env;
	(void)is_forked;
	return (ft_echo(argv + 1));
}

static int	run_cd(char **argv, t_env *env, t_bool is_forked)
{
	(void)is_forked;
	return (ft_cd(argv + 1, env));
}

static int	run_pwd(char **argv, t_env *env, t_bool is_forked)
{
	(void)argv;
	(void)env;
	(void)is_forked;
	return (ft_pwd());
}

static int	run_export(char **argv, t_env *env, t_bool is_forked)
{
	(void)is_forked;
	return (ft_export(argv + 1, env));
}

static int	run_unset(char **argv, t_env *env, t_bool is_forked)
{
	(void)is_forked;
	return (ft_unset(argv + 1, env));
}

static int	run_env(char **argv, t_env *env, t_bool is_forked)
{
	(void)is_forked;
	if (argv[1] != NULL)
		return (ft_error(F_ERROR_ARG, NULL, E_BUILT_IN_FAIL));
	return (ft_env(env));
}

static int	run_exit(char **argv, t_env *env, t_bool is_forked)
{
	(void)env;
	return (ft_exit(argv + 1, is_forked));
}

static int	ft_exe_builtin(char **argv, t_env *env, t_bool is_forked)
{
	static t_builtin_dispatch	dispatch[] = {
	{"echo", run_echo},
	{"cd", run_cd},
	{"pwd", run_pwd},
	{"export", run_export},
	{"unset", run_unset},
	{"env", run_env},
	{"exit", run_exit},
	{NULL, NULL}
	};
	int							i;

	i = 0;
	while (dispatch[i].name != NULL)
	{
		if (ft_strcmp(argv[0], dispatch[i].name) == 0)
			return (dispatch[i].run(argv, env, is_forked));
		i++;
	}
	return (E_BUILT_IN_NO_BUILT);
}

static void	ft_exe_extern(char **av, char **envp, t_env *env)
{
	char	*path;
	char	*target;

	path = env_get(env, "PATH");
	if (path == NULL || ft_str_find(av[0], '/') < ft_strlen(av[0]))
	{
		chk_access_and_exe(av[0], av, envp);
		exit(ft_error(F_ERROR_FILE, av[0], E_EXIT_NO_CMD));
	}
	ft_signal(SIG_DFL, SIG_DFL, 1);
	target = path;
	ft_exe_rel(target, av, envp);
}

pid_t	ft_exe_cmd(char **av, t_env *env, int context)
{
	char	**envp;
	pid_t	last_pid;

	last_pid = ft_exe_builtin(av, env, (context != E_EXE_MAIN));
	if (last_pid == E_BUILT_IN_NO_BUILT)
	{
		envp = env_to_envp(env);
		if (context != E_EXE_PIPE)
		{
			ft_signal(SIG_IGN, SIG_IGN, 0);
			last_pid = ft_fork();
			if (last_pid == 0)
				ft_exe_extern(av, envp, env);
		}
		else
			ft_exe_extern(av, envp, env);
		ft_argv_free(envp);
	}
	return (last_pid);
}
