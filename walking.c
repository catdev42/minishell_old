/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   walking.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 17:53:30 by spitul            #+#    #+#             */
/*   Updated: 2024/10/14 19:04:14 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

static void	print_cmd(t_execcmd *ecmd);
static void	print_redir(t_redircmd *rcmd);
static void	print_pipe(t_pipecmd *pcmd);

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
}

void	walking(struct s_cmd *cmd)
{
	struct s_execcmd	*ecmd;
	struct s_redircmd	*rcmd;
	struct s_pipecmd	*pcmd;

	ecmd = NULL;
	pcmd = NULL;
	rcmd = NULL;
	if (cmd)
	{
		if (cmd->type == 1)
		{
			ecmd = (t_execcmd *)cmd;
			print_cmd(ecmd);
		}
		else if (cmd->type == 2)
		{
			rcmd = (t_redircmd *)cmd;
			print_redir(rcmd);
		}
		else if (cmd->type == 3)
		{
			pcmd = (t_pipecmd *)cmd;
			print_pipe(pcmd);
		}
		else
			return ;
	}
}

static void	print_cmd(t_execcmd *cmd)
{
	printf("EXEC node %d. \n", cmd->type);
	print_tab(cmd->argv);
	printf("ecmd: %p\n\n", cmd->argv);
}
static void	print_redir(t_redircmd *cmd)
{
	printf("REDIR cmd: %d\n", cmd->fd);
	printf("File name:%s. \n", cmd->file);
	walking(cmd->cmd);
}

static void	print_pipe(t_pipecmd *cmd)
{
	printf("PIPE node %d. \n\n", cmd->type);
	walking(cmd->left);
	walking(cmd->right);
}
