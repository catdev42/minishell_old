/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 19:07:28 by spitul            #+#    #+#             */
/*   Updated: 2024/09/22 16:03:00 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
			free_array(env, len_pointers);
			return (NULL);
		}
		i++;
	}
	return (envp); // envp has to be freed before end?
}

char	*get_env_var(char **env, char *var)
{
	int i;
	size_t len;
	char *pos;

	if (!var || !env || !*env)
		return (NULL);
	// exit(EXIT_FAILURE);
	//? myakoven i would prob just return null here
	// i think this should make us go to the next line probably,
	// instead of totally exit
	pos = NULL;
	i = 0;
	len = ft_strlen(var);
	while (env[i] && !pos)
	{
		pos = ft_strnstr(env[i], var, len);
		i++;
	}
	if (pos && *(pos + len) == '=')
		return (pos + len + 1);
	else 
		return (NULL);
}
