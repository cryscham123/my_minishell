#include "utils.h"

void	*ft_argv_free(char **argv)
{
	int	i;

	if (argv == NULL)
		return (NULL);
	i = 0;
	while (argv[i] != NULL)
		free(argv[i++]);
	free(argv);
	return (NULL);
}

void	*ft_zalloc(size_t size)
{
	size_t			i;
	unsigned char	*target;

	target = malloc(size);
	if (target == NULL)
		exit(ft_error(F_ERROR_SYS, "malloc", E_EXIT_FAILURE));
	i = 0;
	while (i < size)
	{
		target[i] = 0;
		i++;
	}
	return (target);
}
