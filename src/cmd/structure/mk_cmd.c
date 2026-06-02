#include "cmd.h"
#include "parse.h"
#include "utils.h"

static t_cmd	*mk_cmd(union u_cmd *specific_cmd, int cmd_info)
{
	t_cmd		*ret;

	ret = ft_zalloc(sizeof(t_cmd));
	ret->cmd_info = cmd_info;
	ret->cmd = specific_cmd;
	return (ret);
}

t_cmd	*mk_pu_cmd(void)
{
	union u_cmd	*specific_cmd;

	specific_cmd = ft_zalloc(sizeof(union u_cmd));
	return (mk_cmd(specific_cmd, 0));
}

t_cmd	*mk_su_cmd(t_cmd *target)
{
	union u_cmd	*specific_cmd;

	specific_cmd = ft_zalloc(sizeof(union u_cmd));
	specific_cmd->su_cmd.target = target;
	return (mk_cmd(specific_cmd, E_T_SUB));
}

t_cmd	*mk_pb_cmd(t_cmd *target)
{
	union u_cmd	*specific_cmd;

	specific_cmd = ft_zalloc(sizeof(union u_cmd));
	specific_cmd->pb_cmd.target = target;
	return (mk_cmd(specific_cmd, E_T_PIPE));
}

t_cmd	*mk_bi_cmd(t_cmd *left, t_cmd *right, int cmd_info)
{
	union u_cmd	*specific_cmd;

	specific_cmd = ft_zalloc(sizeof(union u_cmd));
	specific_cmd->bi_cmd.left = left;
	specific_cmd->bi_cmd.right = right;
	return (mk_cmd(specific_cmd, cmd_info));
}
