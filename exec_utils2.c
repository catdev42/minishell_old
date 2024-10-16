/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 14:55:08 by spitul            #+#    #+#             */
/*   Updated: 2024/10/16 17:31:14 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

/* Checks if a path is a file or directory File: 1; Dir: 2; Neither: 0*/
int	file_dir_noexist(const char *path, int fd_in_or_out)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) != 0)
	{
		if (errno == ENOENT && fd_in_or_out == 1)/* this should be checked only with infiles*/
			return (1);
		print_error(path, strerror(errno), NULL);
		return (0);
	}
	if (S_ISREG(path_stat.st_mode))
	{
		return (3);
	}
	else if (S_ISDIR(path_stat.st_mode))
		return (2);
	else
		print_error(path, "Is neither a file nor a directory", NULL);
	return (0);
}

/* Return the MODE necessary for OPEN() file or dir */

int	check_file_type(t_redircmd *rcmd, int fd_in_or_out, t_tools *tools)
{
	struct s_redircmd* rcmd;
	char	*filepath;
	int		fileordir;

	if (!rcmd || fd_in_or_out < 0)
		return (0);	
	fileordir = file_dir_noexist(rcmd->file, fd_in_or_out);
	if (fileordir == 0)
		return (-1);
	if (rcmd->fd == 1 && fileordir == 2)
		print_error(filepath, "Is a directory", NULL);
	if (fileordir == 1 && rcmd->append && rcmd->fd == 1)
		return (O_WRONLY | O_CREAT | O_APPEND);
	else if (fileordir == 1 && !rcmd->append && rcmd->fd == 1)
		return (O_WRONLY | O_CREAT | O_TRUNC);
	else if (fileordir == 3 && rcmd->fd == 0)
		return (O_RDONLY);
	else if (fileordir == 2 && rcmd->fd == 0)
		return (O_RDONLY | __O_DIRECTORY);
	return (0);
}
