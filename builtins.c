/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 14:13:16 by spitul            #+#    #+#             */
/*   Updated: 2024/10/10 21:09:54 by myakoven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	echo ()
{
	return (1);
}

int	cd()
{
	return (1);
}

int	export ()
{
	return (1);
}

int	unset()
{
	return (1);
}

int	env()
{
	return (1);
}

int	ft_exit()
{
	return (1);
}

int	pwd(cmd_t *cmd)
{
	char *cwd;
	size_t i;

	i = 0;
	cwd = NULL;
	if (get_matrix_len(cmd->cmds) > 1)
		ft_putstr_fd("pwd: too many arguments\n", 2);
	cwd = getcwd(NULL, 0); //should we check for malloc error
	if (cwd != NULL)
	{
		ft_putstr_fd(cwd, 1);
		ft_putstr_fd("\n", 1);
		free (cwd);
	}
	else
		perror("pwd: error retrieving current directory:");
	return (1);
}
