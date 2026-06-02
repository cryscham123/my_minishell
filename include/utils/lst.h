#ifndef LST_H
# define LST_H

# include <stdlib.h>

typedef struct s_lst
{
	char			*data;
	int				info;
	struct s_lst	*nxt;
	struct s_lst	*prev;
}	t_lst;

t_lst	*mk_lst(char *data, int info);
t_lst	*lst_back(t_lst *lst);
void	lst_push(t_lst **target, t_lst *to_push);
void	lst_delete(t_lst *to_del);
t_lst	*mk_str_node(char *s, int info);
void	lst_delete(t_lst *to_del);
void	*ft_node_free(char *data, int info);
void	*ft_lst_free(t_lst *lst);
t_lst	*mk_str_lst(char *s);
size_t	ft_lst_size(t_lst *lst);
char	*ft_c_str(t_lst *s, size_t size);
t_lst	*ft_lst_trim(t_lst *lst);
int		ft_lst_find(t_lst *s, char c);

#endif
