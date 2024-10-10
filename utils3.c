/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 20:33:23 by myakoven          #+#    #+#             */
/*   Updated: 2024/10/10 00:23:08 by myakoven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

/* PRINTS A 2D ARRAY */
int	print_tab(char **envp)
{
	size_t	i;

	i = 0;
	if (!envp || !*envp)
		return (0);
	while (envp[i])
	{
		printf("%s\n", envp[i]);
		i++;
	}
	return (1);
}

int	get_matrix_len(char **matrix)
{
	int	i;

	if (!matrix || !*matrix)
		return (0);
	i = 0;
	while (matrix[i])
		i++;
	return (i);
}

/*
Provide the pointer to line and the index you are currently on
Returns 0 is quotes aren't closed
And Index of second quote if they are closed
*/
int	check_quotes(char *line, int i)
{
	int	j;

	j = 1;
	if (!isquote(line[i]))
		return (i);
	while (line[i + j])
	{
		if (line[i] == line[i + j])
			return (i + j);
		j++;
	}
	print_error(NULL, UNCLOSED, NULL);
	return (0);
}

/*
Returns Index of CLOSING QUOTE if they are closed or END OF LINE
*/
int	skip_quotes(char *line, int i)
{
	int	j;

	j = 1;
	if (!isquote(line[i]))
		return (i);
	while (line[i + j])
	{
		if (line[i] == line[i + j])
			return (i + j);
		j++;
	}
	return (i + j);
}


/* Returns the index of the last character of the token,
	just like skip quote is index of the second quote */
int	skip_token(char *start, int i)
{
	while (start[i] && !isspace(start[i]))
	{
		if (isquote(start[i]))
			i = skip_quotes(start, i);
		i++;
	}
	return (i - 1);
}

/* Checks if a path is a file or directory File: 1; Dir: 2; Neither: 0 */

/*
int	file_dir_noexist(const char *path, int fd_in_or_out)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) != 0)
	{
		if (errno == ENOENT && fd_in_or_out == 1)
			return (1);
		// this should be checked only with infiles
		print_error(path, strerror(errno), NULL);
		return (0);
	}
	if (S_ISREG(path_stat.st_mode))
		return (1);
	else if (S_ISDIR(path_stat.st_mode))
		return (2);
	else
		print_error(path, "Is neither a file nor a directory", NULL);
	return (0);
}
*/
