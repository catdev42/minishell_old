/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 10:01:36 by spitul            #+#    #+#             */
/*   Updated: 2024/10/15 13:30:16 by myakoven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/minishell.h"

/**
 * this needs to checkthe exit code, determine if its a fatal errno, and exit...
 * exit codes in forks carry errno back!

	* in main status errno is caught and we need to determine to close the whole program or not
 */
void	check_system_fail(int status, t_tools *tools)
{
	// PATHVARFAIL  141 is system failure
	//(if we cant find path fariable the whole this is a bust)
	if (WIFEXITED(status))
		tools->exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		tools->exit_code = WTERMSIG(status) + 128;
}

void	change_shlvl(t_tools *tool)
{
	char	*var;
	int		shlvl;

	var = get_var(tool->env, "SHLVL");
	shlvl = ft_atoi(var) + 1;
}

int	is_builtin(char *s)
{
	int	a;

	a = 0;
	if (ft_strncmp(s, ECHO, 5) == 0)
		a = 1;
	else if (ft_strncmp(s, CD, 3) == 0)
		a = 1;
	else if (ft_strncmp(s, PWD, 4) == 0)
		a = 1;
	else if (ft_strncmp(s, EXPORT, 7) == 0)
		a = 1;
	else if (ft_strncmp(s, UNSET, 6) == 0)
		a = 1;
	else if (ft_strncmp(s, ENV, 4) == 0)
		a = 1;
	else if (ft_strncmp(s, EXIT, 5) == 0)
		a = 1;
	return (a);
}

int	run_builtin(char *s)
{
	int	a;

	a = 0;
	if (ft_strncmp(s, ECHO, 5) == 0)
		a = echo();
	else if (ft_strncmp(s, CD, 3) == 0)
		a = cd();
	else if (ft_strncmp(s, PWD, 4) == 0)
		a = pwd();
	else if (ft_strncmp(s, EXPORT, 7) == 0)
		a = export();
	else if (ft_strncmp(s, UNSET, 6) == 0)
		a = unset();
	else if (ft_strncmp(s, ENV, 4) == 0)
		a = env();
	else if (ft_strncmp(s, EXIT, 5) == 0)
		a = ft_exit();
	return (a);
}
