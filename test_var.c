/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_var.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emmmilla <emmmilla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/01 11:21:59 by emmmilla          #+#    #+#             */
/*   Updated: 2026/07/01 11:22:06 by emmmilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#include "get_next_line.h"

static void	read_file(char *filename)
{
	int		fd;
	int		line_no;
	char	*line;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		perror(filename);
		return ;
	}
	printf("\n===== %s =====\n", filename);
	line_no = 1;
	line = get_next_line(fd);
	while (line)
	{
		printf("[%02d] %s", line_no++, line);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
}

int	main(int argc, char **argv)
{
	int	i;

	if (argc < 2)
	{
		printf("Usage: %s <file1> [file2 ... fileN]\n", argv[0]);
		return (1);
	}
	i = 1;
	while (i < argc)
	{
		read_file(argv[i]);
		i++;
	}
	return (0);
}
