/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_freetab.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 16:09:28 by myakoven          #+#    #+#             */
/*   Updated: 2024/10/10 22:12:42 by myakoven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* Put in array and the number of elements you would like to free 
or 0 if you don't know, hopefully the array is NULL terminated in that case */
void	ft_freetab(char **arr, int len)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	if (len <= 0)
		len = INT_MAX;
	while (arr[i] && i < len)
		free(arr[i++]);
	free(arr);
}
