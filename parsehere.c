/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsehere.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 19:16:34 by myakoven          #+#    #+#             */
/*   Updated: 2024/10/20 17:58:55 by myakoven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

// rewrite nullify to use actual information

/* Initialize the heredoc names struct */
void	here_init(char heredocs[MAXARGS][MAXARGS], t_tools *tools)
{
	int		i;
	char	*tempalloc;

	i = 0;
	while (i < MAXARGS)
	{
		tempalloc = NULL;
		tempalloc = ft_atoi(i);
		if (!tempalloc)
			error_exit(tools, errno);
		ft_strlcpy(heredocs[i], "heredoc", MAXARGS);
		ft_strlcat(heredocs[i], tempalloc, MAXARGS);
		free(tempalloc);
	}
	return ;
}
// tools->lastredir;
// launchreadlineloop
// write what it sees into a file and turn it into a regular infile
struct s_cmd	*createredir_here(char *delim, int mode, int fd, t_tools *tools)
{
	char *end;
	char *tempalloc_delim;
	char *line;
	char *templine;
	int fd;

	line = NULL;
	end = NULL;
	end = get_token_end(delim);
	fd = open(tools->heredocs[tools->hereindex++],
			O_WRONLY | O_CREAT | O_APPEND, 0644);
	while (1)
	{
		line = NULL;
		line = readline("heredoc: ");
		if (!line)
			error_exit(tools, errno);
		if (ft_strncmp(line, delim, ft_strlen(delim) + 1) == 0)
			break ;
		templine = safe_calloc(ft_strlen(line) + 3, 1, tools);
		ft_strlcpy(templine, line, ft_strlen(line) + 3);
		ft_strlcat(templine, "\n", ft_strlen(line) + 3);
		write(fd, templine, ft_strlen(templine));
		free(line);
		free(templine);
	}
	close(fd);
	if (tools->lastredir)
	{
		tools->lastredir->cmd = makeredir(filestart, end, mode, fd);
		tools->lastredir = (struct s_redircmd *)tools->lastredir->cmd;
	}
	else
		tools->lastredir = (struct s_redircmd *)makeredir(filestart, end, mode,
				fd);
	if (!tools->lastredir)
		error_exit(tools, 1);
	return ((struct s_cmd *)tools->lastredir);
}