/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 19:22:37 by myakoven          #+#    #+#             */
/*   Updated: 2024/10/14 18:13:47 by myakoven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

/*
Error paths:
Parsing:
	if system error 
		then exit (with cleaning)
	else 
		return 0 in all functions (except -1 in mode finding (MOVE TO EXECUTION))
Execution (FORKS)
		Print error and exit with errno
		Waitpid catches the error, analyses to see if System Failure 

		If CRITICAL ERROR
			(Program should exit immediately as oppposed to just terminate one fork and keep going)
		else if just some error in a fork (like command not found)
			terminate just that fork, keep going with the program		
*/


/* FOR MAIN PROCESS
Input NULL or what to print in the 3 prositions:
msh: (arg): (errline) `(errarg)'
If NULL at a specific position, that position is NOT printed
*/
int	print_error(const char *arg, const char *errline, const char *errarg)
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
	return (0);
}

/* FOR FORK PROCESS
Input NULL if  a parameter is not needed.
//??? What do subsequent exit return is critical error???
ARG: input file name or command name
ERRLINE: custom error message or strerror(errnum) is printed
ERRARG: the thing inside of backticks if needed 
ex: print_errno_exit(, NULL, NULL, tools);*/

int	print_errno_exit(const char *arg, const char *errline, const char *errarg, t_tools *tools)
{
	clean_tools(tools);
	clear_history();
	ft_putstr_fd("msh: ", 2);
	if (arg)
	{
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": ", 2);
	}
	if (errline)
		ft_putstr_fd(errline, 2);
	else
		ft_putstr_fd(strerror(errno), 2);
	if (errarg)
	{
		ft_putstr_fd("`", 2);
		ft_putstr_fd(errarg, 2);
		ft_putstr_fd("\'", 2);
	}
	ft_putstr_fd("\n", 2);
	exit(errno);
}

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
	if (error == 0) // sucessful exit
		exit(0);
	else if (error > 0)
	{
		// usually malloc error.... this need to be edited and replaces with errno exits...
		print_error(NULL, strerror(errno), NULL);
		exit(error);
	}
	else if (error > 1)
		exit(error);
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
// void	print_errnum_error(const char *arg, const char *errline,
// 		const char *errarg, int err)
// {
// 	print_error(const char *arg, const char *errline, const char *errarg);
// 	if (err > 1)
// 		exit(errno);
// }
