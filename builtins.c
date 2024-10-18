/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 14:13:16 by spitul            #+#    #+#             */
/*   Updated: 2024/10/18 19:45:09 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

int	echo(void)
{
	return (1);
}

int	cd(void)
{
	return (1);
}

int	export(void)
{
	return (1);
}

int	unset(void)
{
	return (1);
}

int	env(void)
{
	return (1);
}

int	ft_exit(void)
{
	return (1);
}

int	pwd(t_execcmd *cmd)
{
	char	*cwd;
	size_t	i;

	i = 0;
	cwd = NULL;
	if (get_matrix_len(cmd->argv) > 1)
		ft_putstr_fd("pwd: too many arguments\n", 2);
	cwd = getcwd(NULL, 0); // should we check for malloc error
	if (cwd != NULL)
	{
		ft_putstr_fd(cwd, 1);
		ft_putstr_fd("\n", 1);
		free(cwd);
	}
	else
		perror("pwd: error retrieving current directory:");
	return (1);
}
