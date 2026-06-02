#include "utils.h"
#include "str.h"

void	ft_putstr_fd(char *s, int fd)
{
	size_t	n;

	if (s == NULL)
		return ;
	n = ft_strlen(s);
	if (write(fd, s, n) < 0)
		exit(E_EXIT_FAILURE);
}

int	ft_error(char *msg, char *hint, int exit_status)
{
	char	*tmp_1;
	char	*tmp_2;
	char	*total_1;
	char	*total_2;

	if (isatty(STDERR_FILENO))
		total_1 = ft_append("\e[91mError: \e[0m", msg, '\0');
	else
		total_1 = ft_append("Error: ", msg, '\0');
	if (hint != NULL)
	{
		tmp_1 = ft_append("\'", hint, '\0');
		tmp_2 = ft_append(tmp_1, "\'\n", '\0');
		total_2 = ft_append(total_1, tmp_2, '\0');
		free(tmp_1);
		free(tmp_2);
	}
	else
		total_2 = ft_append(total_1, "\n", '\0');
	ft_putstr_fd(total_2, 2);
	free(total_1);
	free(total_2);
	ft_shell_set_status(exit_status);
	return (exit_status);
}
