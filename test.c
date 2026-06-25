/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emmmilla <emmmilla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/25 12:20:30 by emmmilla          #+#    #+#             */
/*   Updated: 2026/06/25 15:19:59 by emmmilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "get_next_line.h"

// extern void *__real_malloc(size_t c);

// void	*__wrap_malloc(size_t c)
// {
// 	static int counter = 1;
// 	if (counter--)
// 		return (__real_malloc(c));
// 	return (NULL);
// }

static void	print_line(int line_no, char *line)
{
	printf("[%02d] len=%zu | \"%s\"\n",
		line_no, ft_strlen(line), line);
}

int	main(int argc, char **argv)
{
	int		fd;
	int		line_no;
	char	*line;

	if (argc != 2)
	{
		printf("Usage: %s <file>\n", argv[0]);
		return (1);
	}
	fd = open(argv[1], O_RDONLY);
	if (fd < 0)
	{
		perror("open");
		return (1);
	}
	line_no = 1;
	line = get_next_line(fd);
	while (line)
	{
		if (line)
			print_line(line_no++, line);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (0);
}
