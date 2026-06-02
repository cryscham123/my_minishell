#ifndef UTILS_H
# define UTILS_H

# include <unistd.h>
# include <signal.h>
# include <termios.h>
# include <dirent.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>

# define F_ERROR_PWD "Invalid Current Path"
# define F_ERROR_SYS "System Function Failed "
# define F_ERROR_DEVICE "Terminal's Output Device Is Invalid"
# define F_FILE_FAIL "Can't Open File "
# define F_ERROR_FILE "No Such File or Directory "
# define F_ERROR_ARG "Too Many Arguments..."
# define F_ERROR_AMBIG "Ambiguous Redirect "
# define F_ERROR_HEREDOC_FILE "Can't Create File..."
# define F_ERROR_HEREDOC_SET "Heredoc Need Path "
# define F_ERROR_ACCESS "Permission Denied "
# define F_ERROR_NO_DIR "Not A Directory "
# define F_ERROR_NO_FILE "Not A File "
# define F_ERROR_EXE "Command Not Found "
# define F_ERROR_NOT_SUPPORT "Not Supported Meta Word \'&\'"
# define F_ERROR_SYNTAX "Syntax Error Unexpected Token Near "
# define F_ERROR_UNCLOSE "Syntax Error With Unfinished Token"
# define F_ERROR_EXIT "Numeric Argument Required "
# define F_ERROR_KEY "Not A Valid Identifier "

typedef enum e_exit_status
{
	E_EXIT_SUCCESS,
	E_EXIT_FAILURE,
	E_EXIT_SYNTAX,
	E_EXIT_FILE_NOT_EXIST,
	E_EXIT_NO_FILE,
	E_EXIT_FILE_ACCESS,
	E_CMD_NO_FILE = 126,
	E_CMD_ACCESS = 126,
	E_EXIT_NO_CMD,
}	t_exit_status;

typedef enum e_bool
{
	E_FALSE,
	E_TRUE
}	t_bool;

typedef struct s_lst	t_lst;

typedef struct s_env
{
	char	**items;
	size_t	size;
	size_t	capacity;
}	t_env;

typedef struct s_shell
{
	t_env	env;
	int		status;
	t_bool	interactive;
}	t_shell;

int		ft_error(char *msg, char *hint, int exit_status);
void	ft_putstr_fd(char *s, int fd);
char	*ft_terminal_name(t_bool is_set);
void	ft_terminal_restore(void);
void	sigint_handler(int sig);
void	heredoc_signal_handler(int sig);
void	ft_signal(void (*handler_1)(int), void (*handler_2)(int), int flag);
void	ft_shell_set_current(t_shell *shell);
int		ft_shell_status(void);
void	ft_shell_set_status(int status);
t_bool	ft_shell_is_interactive(void);
void	env_init(t_env *env, char **envp);
void	env_free(t_env *env);
char	*env_get(t_env *env, char *key);
int		env_set_entry(t_env *env, char *entry);
int		env_set(t_env *env, char *key, char *value);
void	env_unset(t_env *env, char *key);
char	**env_to_envp(t_env *env);
void	env_print(t_env *env);
void	*ft_zalloc(size_t size);
void	*ft_argv_free(char **argv);

#endif
