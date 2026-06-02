#include "utils.h"
#include "str.h"

static void	env_reserve(t_env *env, size_t capacity)
{
	char	**new_items;
	size_t	i;

	if (env->capacity >= capacity)
		return ;
	new_items = ft_zalloc(sizeof(char *) * capacity);
	i = 0;
	while (i < env->size)
	{
		new_items[i] = env->items[i];
		i++;
	}
	free(env->items);
	env->items = new_items;
	env->capacity = capacity;
}

static size_t	env_key_len(char *entry)
{
	return (ft_str_find(entry, '='));
}

static t_bool	env_key_equals(char *entry, char *key)
{
	size_t	key_len;

	key_len = env_key_len(entry);
	return (key_len == ft_strlen(key) && \
		ft_strncmp(entry, key, key_len) == 0);
}

static int	env_find_index(t_env *env, char *key)
{
	size_t	i;

	i = 0;
	while (i < env->size)
	{
		if (env_key_equals(env->items[i], key) == E_TRUE)
			return (i);
		i++;
	}
	return (-1);
}

static char	*env_increase_shlvl(char *entry)
{
	char	*level;
	char	*level_copy;
	char	*next;
	char	*ret;

	level = entry + env_key_len(entry) + 1;
	if (ft_str_is_num(level) == E_FALSE)
		return (ft_strdup("SHLVL=1"));
	level_copy = ft_strdup(level);
	next = ft_str_plus(level_copy, "1", ft_strlen(level_copy), 1);
	ret = ft_append("SHLVL", next, '=');
	free(level_copy);
	free(next);
	return (ret);
}

void	env_init(t_env *env, char **envp)
{
	char	*entry;

	env->items = NULL;
	env->size = 0;
	env->capacity = 0;
	env_reserve(env, 16);
	while (*envp != NULL)
	{
		if (env_key_equals(*envp, "SHLVL") == E_TRUE)
		{
			entry = env_increase_shlvl(*envp);
			env_set_entry(env, entry);
			free(entry);
		}
		else
			env_set_entry(env, *envp);
		envp++;
	}
	if (env_get(env, "SHLVL") == NULL)
		env_set_entry(env, "SHLVL=1");
}

void	env_free(t_env *env)
{
	size_t	i;

	i = 0;
	while (i < env->size)
	{
		free(env->items[i]);
		i++;
	}
	free(env->items);
	env->items = NULL;
	env->size = 0;
	env->capacity = 0;
}

char	*env_get(t_env *env, char *key)
{
	int		index;
	char	*entry;

	if (key == NULL)
		return (NULL);
	index = env_find_index(env, key);
	if (index < 0)
		return (NULL);
	entry = env->items[index];
	return (entry + env_key_len(entry) + 1);
}

int	env_set_entry(t_env *env, char *entry)
{
	int		index;
	char	*copy;
	char	*key;
	size_t	key_len;

	key_len = env_key_len(entry);
	if (entry[key_len] != '=')
		return (E_EXIT_FAILURE);
	key = ft_substr(entry, key_len);
	index = env_find_index(env, key);
	free(key);
	copy = ft_strdup(entry);
	if (index >= 0)
	{
		free(env->items[index]);
		env->items[index] = copy;
		return (E_EXIT_SUCCESS);
	}
	if (env->size + 1 >= env->capacity)
		env_reserve(env, env->capacity * 2);
	env->items[env->size++] = copy;
	env->items[env->size] = NULL;
	return (E_EXIT_SUCCESS);
}

int	env_set(t_env *env, char *key, char *value)
{
	char	*entry;
	int		ret;

	entry = ft_append(key, value, '=');
	ret = env_set_entry(env, entry);
	free(entry);
	return (ret);
}

void	env_unset(t_env *env, char *key)
{
	int		index;
	size_t	i;

	index = env_find_index(env, key);
	if (index < 0)
		return ;
	free(env->items[index]);
	i = index;
	while (i + 1 < env->size)
	{
		env->items[i] = env->items[i + 1];
		i++;
	}
	env->size--;
	env->items[env->size] = NULL;
}

char	**env_to_envp(t_env *env)
{
	char	**ret;
	size_t	i;

	ret = ft_zalloc(sizeof(char *) * (env->size + 1));
	i = 0;
	while (i < env->size)
	{
		ret[i] = ft_strdup(env->items[i]);
		i++;
	}
	return (ret);
}

void	env_print(t_env *env)
{
	size_t	i;

	i = 0;
	while (i < env->size)
	{
		ft_putstr_fd(env->items[i], 1);
		ft_putstr_fd("\n", 1);
		i++;
	}
}
