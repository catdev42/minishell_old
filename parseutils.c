/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseutils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 19:16:34 by myakoven          #+#    #+#             */
/*   Updated: 2024/10/14 18:53:43 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"


int	infile_or_outfile(char *start)
{
	if (*start == '<')
		return (0);
	else if (*start == '>')
		return (1);
	return (-1);
}



// /* Return the MODE necessary for OPEN() file or dir */
// int	check_file_type(char *start, int fd_in_or_out, t_tools *tools)
// {
// 	char	*filepath;
// 	int		fileordir;

// 	if (!start || fd_in_or_out < 0)
// 		return (0);
// 		struct s_redircmd* rcmd;
// 	// filepath = get_redir_path(start, tools);
// 	fileordir = file_dir_noexist(rcmd->file, fd_in_or_out);
// 	if (fileordir == 0)
// 		return (-1);
// 	if (start[0] == '>' && fileordir == 2)
// 		print_error(filepath, "Is a directory", NULL);
// 	// free(filepath);
// 	if (fileordir == 1 && start[0] == '>' && start[1] == '>')
// 		return (O_WRONLY | O_CREAT | O_APPEND);
// 	else if (fileordir == 1 && start[0] == '>')
// 		return (O_WRONLY | O_CREAT | O_TRUNC);
// 	else if (fileordir == 1 && start[0] == '<')
// 		return (O_RDONLY);
// 	else if (fileordir == 2 && start[0] == '<')
// 		return (O_RDONLY | __O_DIRECTORY);
// 	return (0);
// }
// 	// else if (start[0] == '<' &&start[1] == '<')
// 	// 		; //HEREDOC?

/* Allocates a filename/path. MUST BE FREED */
char	*get_redir_path(char *redir, t_tools *tools)
{
	int		i;
	char	*filename;
	int		start;

	i = 0;
	while (redir[i] && isredir(redir[i]))
	{
		i++;
	}
	start = i;
	while (redir[i] && !ft_isspace(redir[i]))
	{
		if (isquote(redir[i]))
			i = skip_quotes(redir, i);
		i++;
	}
	filename = ft_substr(redir, start, i - start);
	if (!filename)
		error_exit(tools, 1);
	strip_quotes_final(filename);
	return (filename);
}

/* Gets pointer to the space after the token */
char	*get_token_end(char *namestart)
{
	int	i;

	i = 0;
	while (namestart[i] && !ft_isspace(namestart[i]))
	{
		if (isquote(namestart[i]))
			i = skip_quotes(namestart, i);
		i++;
	}
	return (&namestart[i]);
}
