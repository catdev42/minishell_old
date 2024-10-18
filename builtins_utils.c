/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 20:25:45 by spitul            #+#    #+#             */
/*   Updated: 2024/10/18 20:29:03 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

/*checks if here is a non builtin command in a pipefree tree*/
int	builtin_check_walk(t_cmd *cmd)
{
	struct s_execcmd	*ecmd;
	struct s_redircmd	*rcmd;
	int					res;

	ecmd = NULL;
	rcmd = NULL;
	res = 0;
	if (cmd)
	{
		if (cmd->type == EXEC)
		{
			ecmd = (t_execcmd *)cmd;
			res = is_builtin(ecmd->argv[0]);
			return (res);
		}
		else
		{
			rcmd = (t_redircmd *)cmd;
			res = builtin_check_walk(rcmd->cmd);
			return (res);
		}
	}
	return (res);
}
