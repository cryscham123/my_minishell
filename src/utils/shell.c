#include "utils.h"

static t_shell	*g_shell;

void	ft_shell_set_current(t_shell *shell)
{
	g_shell = shell;
}

int	ft_shell_status(void)
{
	if (g_shell == NULL)
		return (0);
	return (g_shell->status);
}

void	ft_shell_set_status(int status)
{
	if (g_shell != NULL)
		g_shell->status = status;
}

t_bool	ft_shell_is_interactive(void)
{
	if (g_shell == NULL)
		return (E_FALSE);
	return (g_shell->interactive);
}
