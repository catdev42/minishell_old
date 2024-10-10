/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 19:07:28 by spitul            #+#    #+#             */
/*   Updated: 2024/10/09 23:54:12 by myakoven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

char	**copy_env(t_tools *tools, char **env)
{
	char	**envp;
	int		len_pointers;
	int		i;

	i = 0;
	len_pointers = 0;
	while (env[len_pointers] != NULL)
		len_pointers++;
	envp = ft_calloc((len_pointers + 5), sizeof(char *));
	if (!envp)
		return (NULL);
	tools->env_len = len_pointers + 3;
	while (i < len_pointers && env[i])
	{
		envp[i] = ft_strdup(env[i]);
		if (!envp[i])
		{
			ft_freetab(env, len_pointers);
			return (NULL);
		}
		i++;
	}
	return (envp);
}

/* Return the pointer to the variable definition or NULL if not found */
char	*get_var(char **env, char *var)
{
	int		i;
	size_t	len;
	char	*line;

	if (!env || !*env)
		return (NULL);
	line = NULL;
	i = 0;
	len = ft_strlen(var);
	while (env[i] && !line)
	{
		line = ft_strnstr(env[i], var, len);
		if (line && line[len] == '=')
			line = line + len + 1;
		else
			line = NULL;
		i++;
	}
	return (line);
}
