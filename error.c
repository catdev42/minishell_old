/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 19:22:37 by myakoven          #+#    #+#             */
/*   Updated: 2024/10/14 18:05:13 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

/*

Input NULL or errline and/or errarg.
If errline is provided, an error is printed to the screen,
is errarg is provided, it is appended to the error string*/
int	print_error(const char *arg, const char *errline, const char *errarg,
		int err)
{
	ft_putstr_fd("msh: ", 2);
	if (arg)
	{
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": ", 2);
	}
	if (errline)
	{
		ft_putstr_fd(errline, 2);
		if (errarg)
		{
			ft_putstr_fd("`", 2);
			ft_putstr_fd(errarg, 2);
			ft_putstr_fd("\'", 2);
		}
		ft_putstr_fd("\n", 2);
	}
	return (errno); // returns exit failure int
}
// void	print_errnum_error(const char *arg, const char *errline,
// 		const char *errarg, int err)
// {
// 	print_error(const char *arg, const char *errline, const char *errarg);
// 	if (err > 1)
// 		exit(errno);
// }
/*
FOR EXITING!
0: CTRL D or EXIT SUCCESS
1: malloc
3: just exit(1) nothing printed
*/

int	error_exit(t_tools *tools, int error)
{
	clean_tools(tools);
	clear_history();
	if (error == 0)
		exit(0);
	else if (error == 1 || error == 6)
	{
		print_error(NULL, strerror(errno), NULL);
		exit(1);
	}
	else if (error == 2)
		exit(1);
	return (1);
}

void	clean_tools(t_tools *tools)
{
	reset_tools(tools);
	if (tools->env)
		ft_freetab(tools->env, INT_MAX);
}

struct s_cmd	*clean_execs(struct s_cmd *first, struct s_cmd *second)
{
	struct s_redircmd	*rcmd;

	if (first)
	{
		if (first->type == REDIR)
		{
			rcmd = (struct s_redircmd *)first;
			free(rcmd);
		}
	}
	if (second)
	{
		if (first->type == REDIR)
		{
			rcmd = (struct s_redircmd *)second;
			free(rcmd);
		}
	}
	return (NULL);
}

/*
// ft_freetab in library

char	**free_array(char **res, int nb)
{
	int	i;

	i = 0;
	if (!res)
		return (NULL);
	while (i <= nb && res[i])
	{
		free(res[i]);
		i++;
	}
	free(res);
	return (NULL);
}
*/
