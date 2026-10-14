/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aferryat <aferryat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 19:59:31 by aferryat          #+#    #+#             */
/*   Updated: 2024/12/15 15:12:29 by aferryat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static int	ft_manager(char *target)
{
	int	index;

	index = get_index(target, '\n');
	if (index == 0 && target[index] != '\n')
		index = get_index (target, '\0');
	else
		index++;
	return (index);
}

static char	*extract_rest(char	*new_res)
{
	char	*result;
	int		index;
	int		i;

	i = 0;
	if (!new_res)
		return (NULL);
	index = ft_manager(new_res);
	result = malloc (ft_strlen(new_res + index) + 1);
	if (result == NULL)
		return (free(new_res), NULL);
	while (new_res[index + i])
	{
		result[i] = new_res[index + i];
		i++;
	}
	result[i] = '\0';
	free(new_res);
	new_res = NULL;
	if (result[0] == '\0')
	{
		free(result);
		return (NULL);
	}
	return (result);
}

static char	*extract_line(char *res)
{
	int		index;
	char	*line;
	int		i;

	i = 0;
	if (res == NULL)
		return (NULL);
	index = ft_manager(res);
	line = malloc(index + 1);
	if (line == NULL)
		return (NULL);
	while (i < index)
	{
		line[i] = res[i];
		i++;
	}
	line[i] = '\0';
	return (line);
}

static char	*fill_line(char	*new_res, int fd)
{
	char	*buffer;
	int		bytes;

	buffer = (char *) malloc ((size_t)BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	while (1)
	{
		bytes = read (fd, buffer, BUFFER_SIZE);
		if (bytes == 0)
			break ;
		else if (bytes < 0)
		{
			free(new_res);
			free(buffer);
			return (NULL);
		}
		buffer[bytes] = '\0';
		new_res = ft_strjoin(new_res, buffer);
		if (get_index(new_res, '\n') > 0)
			break ;
	}
	free(buffer);
	return (new_res);
}

char	*get_next_line(int fd)
{
	static char	*new_res[OPEN_MAX];
	char		*line;

	if (fd > OPEN_MAX)
		return (NULL);
	if (fd < 0 || BUFFER_SIZE <= 0 || BUFFER_SIZE > INT_MAX)
		return (NULL);
	new_res[fd] = fill_line(new_res[fd], fd);
	line = extract_line(new_res[fd]);
	new_res[fd] = extract_rest(new_res[fd]);
	return (line);
}
