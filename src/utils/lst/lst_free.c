#include "lst.h"
#include "utils.h"
#include "parse.h"
#include "files.h"

void	lst_delete(t_lst *to_del)
{
	if (to_del == NULL)
		return ;
	if (to_del->prev != NULL)
		to_del->prev->nxt = to_del->nxt;
	if (to_del->nxt != NULL)
		to_del->nxt->prev = to_del->prev;
	to_del->nxt = NULL;
	ft_lst_free(to_del);
}

void	*ft_node_free(char *data, int info)
{
	if (data == NULL)
		return (NULL);
	if (info == E_T_HEREDOC)
		unlink((char *)data);
	free(data);
	return (NULL);
}

void	*ft_lst_free(t_lst *lst)
{
	t_lst	*tmp;

	while (lst != NULL)
	{
		ft_node_free(lst->data, lst->info);
		tmp = lst;
		lst = lst->nxt;
		free(tmp);
	}
	return (NULL);
}
