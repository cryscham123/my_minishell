#include "parse.h"
#include "cmd.h"

static t_cmd	*cmd_copy(t_cmd *ref)
{
	t_cmd	*ret;

	ret = NULL;
	switch (ref->cmd_info)
	{
		case 0:
			ret = mk_pu_cmd();
			ret->cmd->pu_cmd.argv = ref->cmd->pu_cmd.argv;
			ret->cmd->pu_cmd.redir = ref->cmd->pu_cmd.redir;
			break ;
		case E_T_SUB:
			ret = mk_su_cmd(NULL);
			ret->cmd->su_cmd.target = ref->cmd->su_cmd.target;
			ret->cmd->su_cmd.redir = ref->cmd->su_cmd.redir;
			break ;
		default:
			break ;
	}
	return (ret);
}

void	pipe_cmd_push(t_cmd *ret)
{
	t_cmd	*nxt;
	t_cmd	*new_cmd;

	while (ret != NULL)
	{
		if (ret->cmd_info == E_T_SUB || ret->cmd_info == 0)
		{
			ret->cmd->pb_cmd.target = cmd_copy(ret);
			ret->cmd_info = E_T_PIPE;
			ret->cmd->pb_cmd.nxt = NULL;
		}
		nxt = nxt_cmd(ret);
		if (nxt == NULL)
		{
			new_cmd = mk_pb_cmd(NULL);
			ret->cmd->pb_cmd.nxt = new_cmd;
		}
		ret = nxt;
	}
}

t_parse	ft_sub_split(t_lex **tokens, size_t depth, t_cmd **ret, t_env *env)
{
	t_cmd	*to_push;
	t_cmd	*sub_cmd;

	sub_cmd = parser_build_ast(tokens, depth, env);
	if (sub_cmd == NULL)
		return (E_PARSE_ERROR);
	to_push = mk_su_cmd(sub_cmd);
	cmd_push(ret, to_push);
	return (E_PARSE_PROGRESS);
}
