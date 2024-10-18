/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 17:15:14 by myakoven          #+#    #+#             */
/*   Updated: 2024/10/18 20:32:13 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	full_line(char *line)
{
	int i;

	i = 0;
	while (line[i])
	{
		if (!ft_isspace(line[i]))
			return (1);
		i++;
	}
	return (0);
}

// void	free_tab(char **tab)
// {
// 	int	i;
	
// 	if (!tab || !*tab)
// 		return ;
// 	i = 0;
// 	while (tab[i])
// 	{
// 		free (tab[i]);
// 		i ++;
// 	}
// 	free (tab);
// }
