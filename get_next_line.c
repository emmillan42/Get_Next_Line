/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emmmilla <emmmilla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 12:27:44 by emmmilla          #+#    #+#             */
/*   Updated: 2026/06/23 16:36:11 by emmmilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static char	*read_file(int fd, char *cache);

static char	*extract_line(char *cache);

static char	*clean_cache(char *cache);

char	*get_next_line(int fd)
{
	static char	*cache;
	char		*line;

	cache = read_file(fd, cache);
	if (!cache)
		return (NULL);
	line = extract_line(cache);
	cache = clean_cache(cache);
	return (line);
}
