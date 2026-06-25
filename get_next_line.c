/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emmmilla <emmmilla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 12:27:44 by emmmilla          #+#    #+#             */
/*   Updated: 2026/06/25 08:46:40 by emmmilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*fill_cache(int fd, char *cache, char *buffer)
{
	ssize_t	bytes;

	bytes = 1;
	while (bytes > 0 && !ft_strchr(cache, '\n'))
	{
		bytes = read(fd, buffer, BUFFER_SIZE);
		if (bytes < 0)
		{
			free(cache);
			return (NULL);
		}
		if (bytes > 0)
		{
			buffer[bytes] = '\0';
			cache = ft_strjoin(cache, buffer);
			if (!cache)
				return (NULL);
		}
	}
	return (cache);
}

static char	*read_file(int fd, char *cache)
{
	char	*buffer;

	if (!cache)
	{
		cache = ft_strdup("");
		if (!cache)
			return (NULL);
	}
	buffer = malloc(sizeof(*buffer) * (BUFFER_SIZE + 1));
	if (!buffer)
	{
		free(cache);
		return (NULL);
	}
	cache = fill_cache(fd, cache, buffer);
	free(buffer);
	buffer = NULL;
	return (cache);
}

static char	*extract_line(char *cache)
{
	size_t	i;

	if (!cache || !*cache)
		return (NULL);
	i = 0;
	while (cache[i] && cache[i] != '\n')
		i++;
	if (cache[i] == '\n')
		i++;
	return (ft_substr(cache, 0, i));
}

static char	*clean_cache(char *cache)
{
	char	*new_cache;
	size_t	i;

	i = 0;
	while (cache[i] && cache[i] != '\n')
		i++;
	if (cache[i] == '\0')
	{
		free(cache);
		return (NULL);
	}
	i++;
	if (cache[i] == '\0')
	{
		free(cache);
		return (NULL);
	}
	new_cache = ft_substr(cache, i, ft_strlen(cache) - i);
	if (!new_cache)
	{
		free(cache);
		return (NULL);
	}
	free(cache);
	return (new_cache);
}

char	*get_next_line(int fd)
{
	static char	*cache;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, NULL, 0) < 0)
	{
		free(cache);
		cache = NULL;
		return (NULL);
	}
	cache = read_file(fd, cache);
	if (!cache)
		return (NULL);
	line = extract_line(cache);
	cache = clean_cache(cache);
	return (line);
}
