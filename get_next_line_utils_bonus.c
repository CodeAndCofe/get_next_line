/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aferryat <aferryat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 19:59:50 by aferryat          #+#    #+#             */
/*   Updated: 2024/12/15 20:36:55 by aferryat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

int	ft_strlen(char	*str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

static char	*to_return(char *str1, char *str2)
{
	if (!str1 && str2)
		return (ft_strdup(str2));
	else if (!str2 && str1)
		return (ft_strdup(str1));
	return (NULL);
}

char	*ft_strjoin(char *str1, char *str2)
{
	char	*ptr;
	int		i;
	int		j;

	if (!str1 || !str2)
		return (to_return(str1, str2));
	i = 0;
	j = 0;
	ptr = malloc (ft_strlen(str1) + ft_strlen(str2) + 1);
	if (!ptr)
		return (NULL);
	while (str1[i])
	{
		ptr[i] = str1[i];
		i++;
	}
	while (str2[j])
	{
		ptr[i + j] = str2[j];
		j++;
	}
	free(str1);
	ptr[i + j] = '\0';
	return (ptr);
}

char	*ft_strdup(char *str)
{
	char	*ptr;
	int		i;

	i = 0;
	if (!str)
		return (NULL);
	ptr = malloc(ft_strlen(str) + 1);
	if (ptr == NULL)
		return (NULL);
	while (str[i])
	{
		ptr[i] = str[i];
		i++;
	}
	ptr[i] = '\0';
	return (ptr);
}

int	get_index(char	*str, char c)
{
	int	i;

	i = 0;
	if (str == 0)
		return (-1);
	while (str[i])
	{
		if (str[i] == c)
			return (i);
		i++;
	}
	if (str[i] == c)
		return (i);
	return (0);
}
