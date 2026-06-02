#ifndef FILES_H
# define FILES_H

# include <fcntl.h>
# include <unistd.h>
# include <sys/stat.h>
# include <sys/stat.h>
# include "cmd.h"

# define F_HEREDOC_TEMPLATE "/tmp/minishell_heredoc_XXXXXX"

typedef enum e_dup
{
	E_DUP,
	E_DUP2,
}	t_dup;

int		ft_open(int target_fd[2], int fd[2], char *s, int info);
void	ft_close(int fd[2], int origin_fd[2]);
void	ft_dup(int *fd, int *old_fd, t_dup dup_info);
void	ft_pipe(int fd[2]);
int		ft_redirection(int target_fd[2], int fd[2], t_lst *redir, t_env *env);
t_lst	*ft_heredoc(char *del, t_env *env);

#endif
