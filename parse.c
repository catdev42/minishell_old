/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseline.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 00:42:37 by myakoven          #+#    #+#             */
/*   Updated: 2024/10/09 22:32:52 by myakoven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

static void	nullify(char *cline); // FIX THIS TO BE MORE PRECISE MAYBE?

struct s_cmd	*parseline(char *cline, t_tools *tools)
{
	struct s_cmd	*left;
	struct s_cmd	*right;

	tools->s = cline;
	while (peek(tools->s, tools->e_cline, PIPE))
	{
		left = NULL;
		right = NULL;
		tools->cmd_end = peek(tools->s, tools->e_cline, PIPE);
		left = parseexec(tools->s, tools->cmd_end, tools);
		if (!left)
			return (NULL);
		// ERROR MANAGEMENT:
		// (print error message in parseexec(),
		// give cursor to user in loop)
		tools->s = tools->cmd_end + 1;
		if (peek(tools->s, tools->e_cline, PIPE))
			right = NULL;
		else
		{
			right = parseexec(tools->s, tools->e_cline, tools);
			if (!right)
				return (clean_execs(left, NULL));
			// ERROR MANAGEMENT:
			// (print error message in parseexec(),
			// give cursor to user in loop)
		}
		createpipe(left, right, tools);
		// pipe creation (exits from there if malloc error)
	}
	if (!tools->tree)
		tools->tree = parseexec(tools->s, tools->e_cline, tools);
	nullify(tools->cleanline);
	return (tools->tree);
}

/* Helper for parseline and pipe creation: determines where to attach the pipe */
struct s_cmd	*createpipe(struct s_cmd *left, struct s_cmd *right,
		t_tools *tools)
{
	if (tools->lastpipe)
	{
		tools->lastpipe->right = makepipe(left, right);
		tools->lastpipe = (struct s_pipecmd *)tools->lastpipe->right;
	}
	else
		tools->lastpipe = (struct s_pipecmd *)makepipe(left, right);
	if (!tools->lastpipe)
	{
		clean_execs(left, right);
		error_exit(tools, 1); // EXITS ENTIRE PROGRAM ON ALLOCATION ERROR!
	}
	if (!tools->tree)
		tools->tree = (struct s_cmd *)tools->lastpipe;
	return ((struct s_cmd *)tools->lastpipe);
}

char	*peek(char *line, char *end, int token)
{
	char	*tokenaddress;
	int		i;

	tokenaddress = 0;
	i = 0;
	while (line[i] && &line[i] < end)
	{
		if (isquote(line[i]))
			i = skip_quotes(line, i);
		if (istoken(line[i]))
		{
			if (isredir(line[i]) && token == REDIR)
			{
				tokenaddress = &line[i];
				break ;
			}
			if (line[i] == '|' && token == PIPE)
			{
				tokenaddress = &line[i];
				break ;
			}
		}
		i++;
	}
	return (tokenaddress);
}

static void	nullify(char *cline)
{
	int	i;

	i = 0;
	while (cline[i])
	{
		if (isspace(cline[i]))
			cline[i] = 0;
		else
			i = skip_token(cline, i);
		i++;
	}
}
